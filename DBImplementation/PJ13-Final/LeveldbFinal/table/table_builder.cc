// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "leveldb/table_builder.h"

#include <assert.h>
#include "leveldb/comparator.h"
#include "leveldb/env.h"
#include "leveldb/filter_policy.h"
#include "leveldb/options.h"
#include "table/block_builder.h"
#include "table/filter_block.h"
#include "table/format.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {

    struct TableBuilder::Rep {
// data_block 相关:
        Options options;
        BlockBuilder data_block;    // 用于构建当前data block, flush 之后会清空
// index_block 相关:
        Options index_block_options; // =options, 且 block_restart_interval=1
        BlockBuilder index_block;    // index_block_options

// filter_block:
        FilterBlockBuilder *filter_block;   //

        WritableFile *file;         // SSTable 文件句柄
        uint64_t offset;            // 初始化为 0，记录当前 SSTable 的写入位置

        // 虽然说每次 TableBuilder::Add() 调用都会更改 last_key 值，但是真正用的地方是和 index_block 相关的
        std::string last_key;       // 用于保证每次添加的 key 是按 copmparator 递增的
        Status status;
        int64_t num_entries;        // 记录 SSTable 构建过程中 entry 个数
        bool closed;                // false，直到  Finish() 或 Abandon() 调用才改为 true

        // 每个 data_block 达到大小阈值时,就应该写入文件，
        // 然后会产生一个 BlockHandle 来记录这个 data_block 在 file 中的起始位置和长度
        // 紧接着就把这个 BlockHandle 以 <data_block->last_key, BlockHandle> 格式写入到 index_block 当中，
        // ! 但是这里并不是这么快就执行这个 <data_block->last_key, BlockHandle> 写入的
        // 原因是 data_block->last_key 可能过长，浪费空间，需要等待下个 key 进来，
        // 我们需要找到 data_block->last_key 和 下个 key 之间，尽可能大于等于 data_block->last_key 并且最短的 key
        // 之后才会 <data_block->new_last_key, BlockHandle> 写入 index_block

        // 所以才会需要下面两个变量来记录相关值
        bool pending_index_entry;
        BlockHandle pending_handle;

        std::string compressed_output;  // 用于存放 data_block 压缩数据，每次 flush 之后清空

        Rep(const Options &opt, WritableFile *f)
                : options(opt),
                  index_block_options(opt),
                  file(f),
                  offset(0),
                  data_block(&options),
                  index_block(&index_block_options),
                  num_entries(0),
                  closed(false),
                  filter_block(opt.filter_policy == nullptr ? nullptr
                                                            : new FilterBlockBuilder(opt.filter_policy)),
                  pending_index_entry(false) {
            index_block_options.block_restart_interval = 1;
        }
    };

    TableBuilder::TableBuilder(const Options &options, WritableFile *file)
            : rep_(new Rep(options, file)) {
        if (rep_->filter_block != nullptr) {
            rep_->filter_block->StartBlock(0);
        }
    }

    TableBuilder::~TableBuilder() {
        assert(rep_->closed);  // Catch errors where caller forgot to call Finish()
        delete rep_->filter_block;
        delete rep_;
    }

    Status TableBuilder::ChangeOptions(const Options &options) {
        // Note: if more fields are added to Options, update
        // this function to catch changes that should not be allowed to
        // change in the middle of building a Table.
        if (options.comparator != rep_->options.comparator) {
            return Status::InvalidArgument("changing comparator while building table");
        }

        // Note that any live BlockBuilders point to rep_->options and therefore
        // will automatically pick up the updated options.
        rep_->options = options;
        rep_->index_block_options = options;
        rep_->index_block_options.block_restart_interval = 1;
        return Status::OK();
    }

    // 添加一个 entry 进 data_block
    // r->pending_handle.EncodeTo(&handle_encoding);
    // r->index_block.Add(r->last_key, Slice(handle_encoding));
    // r->pending_index_entry = false;
    // r->filter_block->AddKey(key);
    // Flush();
    void TableBuilder::Add(const Slice &key, const Slice &value) {
        Rep *r = rep_;
        assert(!r->closed);
        if (!ok()) return;
        if (r->num_entries > 0) {   // 保证每次添加的key都是有序的
            assert(r->options.comparator->Compare(key, Slice(r->last_key)) > 0);
        }

        // 新的 data_block 初始化时，触发 index_block 写入上一个 data_block 的 BlockHandle
        if (r->pending_index_entry) {   // 初始为 false
            assert(r->data_block.empty());
            //
            r->options.comparator->FindShortestSeparator(&r->last_key, key);
            std::string handle_encoding;
            r->pending_handle.EncodeTo(&handle_encoding);
            r->index_block.Add(r->last_key, Slice(handle_encoding));
            r->pending_index_entry = false;
        }

        if (r->filter_block != nullptr) {
//            r->filter_block->AddKey(key);   // 向布隆过滤器添加值
            // 向布隆过滤器添加mail（value），而不是key
            uint64_t seq = 1;
            std::string tmp_value = value.ToString();
            PutFixed64(&tmp_value, seq);
            r->filter_block->AddKey(Slice(tmp_value));
        }

        r->last_key.assign(key.data(), key.size());     // 更新 last_key
        r->num_entries++;
        r->data_block.Add(key, value);

        // 当前 data_block 大小达到 options 定义的 4MB，就刷一次盘
        const size_t estimated_block_size = r->data_block.CurrentSizeEstimate();
        if (estimated_block_size >= r->options.block_size) {
            Flush();
        }
    }

    // 调用  WriteBlock(&r->data_block, &r->pending_handle);
    // 刷新 file 缓冲区
    // r->pending_index_entry = true;
    // r->filter_block->StartBlock(r->offset);
    void TableBuilder::Flush() {
        Rep *r = rep_;
        assert(!r->closed);
        if (!ok()) return;
        if (r->data_block.empty()) return;
        assert(!r->pending_index_entry);
        WriteBlock(&r->data_block, &r->pending_handle);
        if (ok()) {
            r->pending_index_entry = true;
            r->status = r->file->Flush();   // WritableFile 有个缓冲区，该函数刷新缓冲区进文件
        }
        if (r->filter_block != nullptr) {
            r->filter_block->StartBlock(r->offset);
        }
    }

    // 获取 block_contents，
    // 若压缩，压缩后的值写入 rep_.compressed_output
    // 调用 WriteRawBlock(block_contents, type, rep_.pending_handle)
    // rep_.compressed_output 清空
    // rep_.data_block 清空
    void TableBuilder::WriteBlock(BlockBuilder *block, BlockHandle *handle) {
        // File format contains a sequence of blocks where each block has:
        //    block_data: uint8[n]
        //    type: uint8
        //    crc: uint32
        assert(ok());
        Rep *r = rep_;
        Slice raw = block->Finish();    // 该函数返回 BlockBuilder 缓冲区内容

        Slice block_contents;
        CompressionType type = r->options.compression;
        // TODO(postrelease): Support more compression options: zlib?
        switch (type) {
            case kNoCompression:
                block_contents = raw;
                break;

            case kSnappyCompression: {
                std::string *compressed = &r->compressed_output;
                if (port::Snappy_Compress(raw.data(), raw.size(), compressed) &&
                    compressed->size() < raw.size() - (raw.size() / 8u)) {
                    block_contents = *compressed;
                } else {
                    // Snappy not supported, or compressed less than 12.5%, so just
                    // store uncompressed form
                    block_contents = raw;
                    type = kNoCompression;
                }
                break;
            }
        }
        WriteRawBlock(block_contents, type, handle);
        r->compressed_output.clear();
        block->Reset();
    }


    // pending_handle
    // 写入 data block 的 block_contents
    // 构建 data block 的 type+crc，
    // 写入 type+crc，
    // 更新rep_.offset
    void TableBuilder::WriteRawBlock(const Slice &block_contents,
                                     CompressionType type,
                                     BlockHandle *handle) {
        Rep *r = rep_;
        handle->set_offset(r->offset);
        handle->set_size(block_contents.size());
        r->status = r->file->Append(block_contents);
        if (r->status.ok()) {   // 每个data block 数据块后是有一个五字节的空间用于存放 type + crc
            char trailer[kBlockTrailerSize];
            trailer[0] = type;
            uint32_t crc = crc32c::Value(block_contents.data(), block_contents.size());
            crc = crc32c::Extend(crc, trailer, 1);  // Extend crc to cover block type
            EncodeFixed32(trailer + 1, crc32c::Mask(crc));
            r->status = r->file->Append(Slice(trailer, kBlockTrailerSize));     // append 函数看情况写入 buffer 或是 直接写入文件
            if (r->status.ok()) {
                r->offset += block_contents.size() + kBlockTrailerSize;
            }
        }
    }

    Status TableBuilder::status() const {
        return rep_->status;
    }

    Status TableBuilder::Finish() {
        Rep *r = rep_;
        Flush();
        assert(!r->closed);
        r->closed = true;

        BlockHandle filter_block_handle, metaindex_block_handle, index_block_handle;

        // Write filter block
        if (ok() && r->filter_block != nullptr) {
            WriteRawBlock(r->filter_block->Finish(), kNoCompression,
                          &filter_block_handle);
        }

        // Write metaindex block
        if (ok()) {
            BlockBuilder meta_index_block(&r->options);
            if (r->filter_block != nullptr) {
                // Add mapping from "filter.Name" to location of filter data
                std::string key = "filter.";
                key.append(r->options.filter_policy->Name());
                std::string handle_encoding;
                filter_block_handle.EncodeTo(&handle_encoding);
                meta_index_block.Add(key, handle_encoding);
            }

            // TODO(postrelease): Add stats and other meta blocks
            WriteBlock(&meta_index_block, &metaindex_block_handle);
        }

        // Write index block
        if (ok()) {
            if (r->pending_index_entry) {
                r->options.comparator->FindShortSuccessor(&r->last_key);
                std::string handle_encoding;
                r->pending_handle.EncodeTo(&handle_encoding);
                r->index_block.Add(r->last_key, Slice(handle_encoding));
                r->pending_index_entry = false;
            }
            WriteBlock(&r->index_block, &index_block_handle);
        }

        // Write footer
        if (ok()) {
            Footer footer;
            footer.set_metaindex_handle(metaindex_block_handle);
            footer.set_index_handle(index_block_handle);
            std::string footer_encoding;
            footer.EncodeTo(&footer_encoding);
            r->status = r->file->Append(footer_encoding);
            if (r->status.ok()) {
                r->offset += footer_encoding.size();
            }
        }
        return r->status;
    }

    void TableBuilder::Abandon() {
        Rep *r = rep_;
        assert(!r->closed);
        r->closed = true;
    }

    // DBImpl::FinishCompactionOutputFile() 中打日志使用
    // DBImpl::DoCompactionWork() 中为文件创建 smallest_key 使用
    uint64_t TableBuilder::NumEntries() const {
        return rep_->num_entries;
    }

    uint64_t TableBuilder::FileSize() const {
        return rep_->offset;
    }

}  // namespace leveldb
