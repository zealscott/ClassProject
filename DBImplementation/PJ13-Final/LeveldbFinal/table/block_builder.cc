// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// BlockBuilder generates blocks where keys are prefix-compressed:
//
// When we store a key, we drop the prefix shared with the previous
// string.  This helps reduce the space requirement significantly.
// Furthermore, once every K keys, we do not apply the prefix
// compression and store the entire key.  We call this a "restart
// point".  The tail end of the block stores the offsets of all of the
// restart points, and can be used to do a binary search when looking
// for a particular key.  Values are stored as-is (without compression)
// immediately following the corresponding key.
//
// An entry for a particular key-value pair has the form:
//     shared_bytes: varint32
//     unshared_bytes: varint32
//     value_length: varint32
//     key_delta: char[unshared_bytes]
//     value: char[value_length]
// shared_bytes == 0 for restart points.
//
// The trailer of the block has the form:
//     restarts: uint32[num_restarts]
//     num_restarts: uint32
// restarts[i] contains the offset within the block of the ith restart point.

#include "table/block_builder.h"

#include <algorithm>
#include <assert.h>
#include "leveldb/comparator.h"
#include "leveldb/table_builder.h"
#include "util/coding.h"

namespace leveldb {

    BlockBuilder::BlockBuilder(const Options *options)
            : options_(options),
              restarts_(),
              counter_(0),
              finished_(false) {
        assert(options->block_restart_interval >= 1);
        restarts_.push_back(0);       // First restart point is at offset 0
    }

    // TableBuilder::Flush() -> TableBuilder::WriteBlock() 处调用
    // TableBuilder 拥有一个 block_builder 对象，每次写满之后，会刷盘，然后调用 BlockBuilder::Reset()
    void BlockBuilder::Reset() {
        buffer_.clear();
        restarts_.clear();
        restarts_.push_back(0);       // First restart point is at offset 0
        counter_ = 0;
        finished_ = false;
        last_key_.clear();
    }

    // 用于 TableBuilder::Add() 中判断是否该block已满，达到刷盘条件
    size_t BlockBuilder::CurrentSizeEstimate() const {
        return (buffer_.size() +                        // Raw data buffer
                restarts_.size() * sizeof(uint32_t) +   // Restart array
                sizeof(uint32_t));                      // Restart array length
    }


    // TableBuilder::WriteBlock() 刷盘之前，肯定要构建完整一个 block
    // ，添加对应的 restart[i] 和 num_restarts
    // 返回完整的 BlockBuilder::buffer_
    Slice BlockBuilder::Finish() {
        // Append restart array
        for (size_t i = 0; i < restarts_.size(); i++) {
            PutFixed32(&buffer_, restarts_[i]);
        }
        PutFixed32(&buffer_, restarts_.size());
        finished_ = true;
        return Slice(buffer_);
    }

    void BlockBuilder::Add(const Slice &key, const Slice &value) {
        Slice last_key_piece(last_key_);
        assert(!finished_);
        assert(counter_ <= options_->block_restart_interval);
        assert(buffer_.empty() // No values yet?
        // 当 block 第一次调用时，buffer 为空，last_key_ 也未初始化，所以后面一个判断条件不用执行
        // buffer 非空时，保证了每次添加进来的 key 都要大于前面的 key
        // 其实前面 TableBuilder::Add() 里面已经验证过了
               || options_->comparator->Compare(key, last_key_piece) > 0);

//  block 格式：
//        string       string      Fixedint32       Fixedint32    Fixedint32
//        +--------------+--------------+--------------+--------------+-----------------+
//        | entries[0]   | entries[n]   | restart[0]   | restart[n]   | num_restarts    |
//        +--------------+--------------+--------------+--------------+-----------------+


        // 计算当前添加进来的 key ，和上次添加进来的 key 的公共前缀长度
        size_t shared = 0;
        // options_ 是当前对象的成员变量，即每个 block 都会拥有一个options
        // 为什么呢？
        // TableBuilder 中一共构造了三种 block
        // 构建 index_block 时，会将 this->options_->block_restart_interval = 1
        if (counter_ < options_->block_restart_interval) {
            // See how much sharing to do with previous string
            const size_t min_length = std::min(last_key_piece.size(), key.size());
            while ((shared < min_length) && (last_key_piece[shared] == key[shared])) {
                shared++;
                // 如果这里 shared == 0 呢？是否添加 restart point?
                // no! 只有 counter 达到了阈值才会触发下面的 else分之，然后去设置 restart point
            }
        } else {
            // Restart compression
            restarts_.push_back(buffer_.size());
            counter_ = 0;
        }
        const size_t non_shared = key.size() - shared;

//  entry 格式：
//        varint32   varint32       varint32         string        string
//        +----------+--------------+----------------+-------------+----------+
//        | shared   | non_shared   | value_length   | key_delta   | value    |
//        +----------+--------------+----------------+-------------+----------+

        // Add "<shared><non_shared><value_size>" to buffer_
        PutVarint32(&buffer_, shared);
        PutVarint32(&buffer_, non_shared);
        PutVarint32(&buffer_, value.size());

        // Add string delta to buffer_ followed by value
        buffer_.append(key.data() + shared, non_shared);
        buffer_.append(value.data(), value.size());

        // Update state
        // 这里更新 last_key_ = key
        // 为什么不直接用上面一句呢？要这么费劲的去构造它，然后还判断是否和 key 相等
        // 可能是为了验证前面做前缀压缩的正确性吧
        last_key_.resize(shared);       // 这里 last_key_.size() > shared, resize 只是截去了 shared 后面的部分，公共部分还是在的。
        last_key_.append(key.data() + shared, non_shared);
        assert(Slice(last_key_) == key);
        counter_++;
    }

}  // namespace leveldb
