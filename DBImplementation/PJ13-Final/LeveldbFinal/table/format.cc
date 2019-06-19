// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "table/format.h"

#include "leveldb/env.h"
#include "port/port.h"
#include "table/block.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {

    void BlockHandle::EncodeTo(std::string *dst) const {
        // Sanity check that all fields have been set
        assert(offset_ != ~static_cast<uint64_t>(0));
        assert(size_ != ~static_cast<uint64_t>(0));
        PutVarint64(dst, offset_);
        PutVarint64(dst, size_);
    }

    // 传入编码后的 BlockHandle
    Status BlockHandle::DecodeFrom(Slice *input) {

        // GetVarint64 会将 input.data指针前移
        if (GetVarint64(input, &offset_) &&
            GetVarint64(input, &size_)) {
            return Status::OK();
        } else {
            return Status::Corruption("bad block handle");
        }
    }

    // TableBuilder::Finish() 处调用
    // dst 为未初始化的空 string
    // 之前存在过疑问，metaindex_handle_ 和 index_handle_ 采用变长编码，
    // 也就是说，可能占不满 40 字节，那么会不会影响解码？ 因为解码都是从 file.size()-48 处开始解码。
    // 这里采用的是  dst->resize(48), 一定会占用 48 字节，用不完的使用 '\0' 填充
    // resize() 和 reserve() 还是有区别的
    void Footer::EncodeTo(std::string *dst) const {
        const size_t original_size = dst->size();
        metaindex_handle_.EncodeTo(dst);
        index_handle_.EncodeTo(dst);
        dst->resize(2 * BlockHandle::kMaxEncodedLength);  // Padding
        PutFixed32(dst, static_cast<uint32_t>(kTableMagicNumber & 0xffffffffu));
        PutFixed32(dst, static_cast<uint32_t>(kTableMagicNumber >> 32));
        assert(dst->size() == original_size + kEncodedLength);
        (void) original_size;  // Disable unused variable warning.
    }

    Status Footer::DecodeFrom(Slice *input) {
        const char *magic_ptr = input->data() + kEncodedLength - 8;
        const uint32_t magic_lo = DecodeFixed32(magic_ptr);
        const uint32_t magic_hi = DecodeFixed32(magic_ptr + 4);
        const uint64_t magic = ((static_cast<uint64_t>(magic_hi) << 32) |
                                (static_cast<uint64_t>(magic_lo)));
        if (magic != kTableMagicNumber) {
            return Status::Corruption("not an sstable (bad magic number)");
        }

        Status result = metaindex_handle_.DecodeFrom(input);
        if (result.ok()) {
            result = index_handle_.DecodeFrom(input);
        }
        if (result.ok()) {
            // We skip over any leftover data (just padding for now) in "input"
            const char *end = magic_ptr + 8;
            *input = Slice(end, input->data() + input->size() - end);
        }
        return result;
    }

    Status ReadBlock(RandomAccessFile *file,
                     const ReadOptions &options,
                     const BlockHandle &handle,
                     BlockContents *result) {
        // 初始化 BlockContents *result
        result->data = Slice();
        result->cachable = false;
        result->heap_allocated = false;

        // Read the block contents as well as the type/crc footer.
        // See table_builder.cc for the code that built this structure.

        // 同时读取 block 块的内容和 type/crc
        // 具体结构会在 table_builder.cc 中讲到
        size_t n = static_cast<size_t>(handle.size());
        char *buf = new char[n + kBlockTrailerSize];
        Slice contents;
        Status s = file->Read(handle.offset(), n + kBlockTrailerSize, &contents, buf);
        if (!s.ok()) {
            delete[] buf;
            return s;
        }
        if (contents.size() != n + kBlockTrailerSize) {
            delete[] buf;
            return Status::Corruption("truncated block read");
        }

        // Check the crc of the type and the block contents
        // 校验 type 和 crc
        const char *data = contents.data();    // Pointer to where Read put the data
        if (options.verify_checksums) {
            const uint32_t crc = crc32c::Unmask(DecodeFixed32(data + n + 1));
            const uint32_t actual = crc32c::Value(data, n + 1);
            if (actual != crc) {
                delete[] buf;
                s = Status::Corruption("block checksum mismatch");
                return s;
            }
        }

        // 判断数据是否压缩，并进行相应解压缩
        switch (data[n]) {
            case kNoCompression:
                if (data != buf) {
                    // File implementation gave us pointer to some other data.
                    // Use it directly under the assumption that it will be live
                    // while the file is open.
                    delete[] buf;
                    result->data = Slice(data, n);
                    result->heap_allocated = false;
                    result->cachable = false;  // Do not double-cache
                } else {
                    result->data = Slice(buf, n);
                    result->heap_allocated = true;
                    result->cachable = true;
                }

                // Ok
                break;
            case kSnappyCompression: {
                size_t ulength = 0;
                if (!port::Snappy_GetUncompressedLength(data, n, &ulength)) {
                    delete[] buf;
                    return Status::Corruption("corrupted compressed block contents");
                }
                char *ubuf = new char[ulength];
                if (!port::Snappy_Uncompress(data, n, ubuf)) {
                    delete[] buf;
                    delete[] ubuf;
                    return Status::Corruption("corrupted compressed block contents");
                }
                delete[] buf;
                result->data = Slice(ubuf, ulength);
                result->heap_allocated = true;
                result->cachable = true;
                break;
            }
            default:
                delete[] buf;
                return Status::Corruption("bad block type");
        }

        return Status::OK();
    }

}  // namespace leveldb
