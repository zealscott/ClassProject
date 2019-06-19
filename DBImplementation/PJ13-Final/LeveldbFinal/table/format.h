// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_FORMAT_H_
#define STORAGE_LEVELDB_TABLE_FORMAT_H_

#include <string>
#include <stdint.h>
#include "leveldb/slice.h"
#include "leveldb/status.h"
#include "leveldb/table_builder.h"

namespace leveldb {

    class Block;

    class RandomAccessFile;

    struct ReadOptions;

// BlockHandle is a pointer to the extent of a file that stores a data
// block or a meta block.
    class BlockHandle {
    public:
        BlockHandle();

        // The offset of the block in the file.
        uint64_t offset() const { return offset_; }
        void set_offset(uint64_t offset) { offset_ = offset; }

        // The size of the stored block
        uint64_t size() const { return size_; }
        void set_size(uint64_t size) { size_ = size; }

        void EncodeTo(std::string *dst) const;

        Status DecodeFrom(Slice *input);

        // 将 offset_ 和 size_ 采用变长编码至少要 20 字节
        enum {
            kMaxEncodedLength = 10 + 10
        };

    private:
        uint64_t offset_;
        uint64_t size_;
    };

// Footer encapsulates the fixed information stored at the tail
// end of every table file.
    class Footer {
    public:
        Footer() {}

        // The block handle for the metaindex block of the table
        const BlockHandle &metaindex_handle() const { return metaindex_handle_; }

        void set_metaindex_handle(const BlockHandle &h) { metaindex_handle_ = h; }

        // The block handle for the index block of the table
        const BlockHandle &index_handle() const {
            return index_handle_;
        }

        void set_index_handle(const BlockHandle &h) {
            index_handle_ = h;
        }

        // TableBuilder::Finish() 处调用
        // dst 为未初始化的空 string
        // 之前存在过疑问，metaindex_handle_ 和 index_handle_ 采用变长编码，
        // 也就是说，可能占不满 40 字节，那么会不会影响解码？ 因为解码都是从 file.size()-48 处开始解码。
        // 这里采用的是  dst->resize(48), 一定会占用 48 字节，用不完的使用 '\0' 填充
        // resize() 和 reserve() 还是有区别的
        void EncodeTo(std::string *dst) const;

        Status DecodeFrom(Slice *input);

        // Encoded length of a Footer.  Note that the serialization of a
        // Footer will always occupy exactly this many bytes.  It consists
        // of two block handles and a magic number.
        // 一个 Footer共 20*2+8 = 48 字节
        enum {
            kEncodedLength = 2 * BlockHandle::kMaxEncodedLength + 8
        };

    private:
        BlockHandle metaindex_handle_;
        BlockHandle index_handle_;
    };

// kTableMagicNumber was picked by running
//    echo http://code.google.com/p/leveldb/ | sha1sum
// and taking the leading 64 bits.

    static const uint64_t kTableMagicNumber = 0xdb4775248b80fb57ull;

    // 1-byte type + 32-bit crc
    // 用于 data_block,
    // data_block 由 BlockContents.data + 1 byte 压缩标志 + 32 bits crc 组成
    static const size_t kBlockTrailerSize = 5;

    struct BlockContents {
        Slice data;           // Actual contents of data
        bool cachable;        // True iff data can be cached
        bool heap_allocated;  // True iff caller should delete[] data.data()
    };

// Read the block identified by "handle" from "file".  On failure
// return non-OK.  On success fill *result and return OK.
    Status ReadBlock(RandomAccessFile *file,
                     const ReadOptions &options,
                     const BlockHandle &handle,
                     BlockContents *result);

// Implementation details follow.  Clients should ignore,

    inline BlockHandle::BlockHandle()
            : offset_(~static_cast<uint64_t>(0)),
              size_(~static_cast<uint64_t>(0)) {
    }

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_FORMAT_H_
