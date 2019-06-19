// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_BUILDER_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_BUILDER_H_

#include <vector>

#include <stdint.h>
#include "leveldb/slice.h"

namespace leveldb {

    struct Options;

    class BlockBuilder {
    public:
        // 调用者传进来的是 DBImpl 中的 options
        // options 提供了以下参数:
        //      block_restart_interval
        //      comparator  (从 DBImpl 调用来看，应该是 InternalKeyComparator )
        explicit BlockBuilder(const Options *options);

        // TableBuilder::Flush() -> TableBuilder::WriteBlock() 处调用
        // TableBuilder 拥有一个 block_builder 对象，每次写满之后，会刷盘，然后调用 BlockBuilder::Reset()
        void Reset();

        // REQUIRES: Finish() has not been called since the last call to Reset().
        // REQUIRES: key is larger than any previously added key
        void Add(const Slice &key, const Slice &value);

        // TableBuilder::WriteBlock() 刷盘之前，肯定要构建完整一个 block
        // ，添加对应的 restart[i] 和 num_restarts
        // 返回完整的 BlockBuilder::buffer_
        Slice Finish();

        // 用于 TableBuilder::Add() 中判断是否该block已满，达到刷盘条件
        size_t CurrentSizeEstimate() const;

        // Return true iff no entries have been added since the last Reset()
        bool empty() const {
            return buffer_.empty();
        }



    private:

        // options_->block_restart_interval 会针对 block 类型作更改
        const Options *options_;
        std::string buffer_;                // 存放当前 block 的值，每次 Add() 都会往后追加，知道调用finish()，才返回这个 buffer_
        std::vector<uint32_t> restarts_;    // Restart points
        int counter_;                       // 用于计数当前多少 entry 共享了同一个前缀，超过 options_->block_restart_interval 会置零重新计数
        bool finished_;                     // Has Finish() been called?
        std::string last_key_;              // 上次 add 进来的 key 的完整值

        // No copying allowed
        BlockBuilder(const BlockBuilder &);
        void operator=(const BlockBuilder &);
    };

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_BUILDER_H_
