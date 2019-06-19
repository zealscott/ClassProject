// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// 阅读 TableBuilder 代码就知道，构建 table 共涉及到了四种 block：
//  1. data block
//  2. filter block
//  3. meta index block
//  4. index block (或称 data index block)
// 其中用于过滤器的 block 在 filter_block.h 中定义,剩下的三种block都同意使用的该文件下的 class block,
// 关于 table_blcok的介绍在笔记里面有讲到，涉及到 restart point 等复杂内容，这里不多讲，知道就行
// 三种 block 构建时都是通过 BlockBuilder::Add() 往里面添加值的，分别是：
//  1. <InternalKey key                 , Slice value>
//  2. <InternalKey data_block->last_key, BlockHandle data_block_handle>
//  3. <string filter_name              , BlockHandle filter_block_handle>
// 不管怎样，都是以 key/value 的形式往里面添加的

// 该文件用于从文件中读取出某个 block，
// 然后实现 内部类迭代器，类用于解析上面添加进来的 entry,
// 需要注意的是，该迭代器的实现不是针对哪一种 block，而是一个通用的工具

#ifndef STORAGE_LEVELDB_TABLE_BLOCK_H_
#define STORAGE_LEVELDB_TABLE_BLOCK_H_

#include <stddef.h>
#include <stdint.h>
#include "leveldb/iterator.h"

namespace leveldb {

    struct BlockContents;
    /*struct BlockContents {
        Slice data;           // Actual contents of data
        bool cachable;        // True iff data can be cached
        bool heap_allocated;  // True iff caller should delete[] data.data()
    };*/

    class Comparator;

    class Block {
    public:
        // Initialize the block with the specified contents.
        explicit Block(const BlockContents &contents);

        ~Block();

        size_t size() const { return size_; }

        Iterator *NewIterator(const Comparator *comparator);

    private:
//  block 格式：
//        string       string      Fixedint32       Fixedint32    Fixedint32
//        +------------+------------+--------------+--------------+-----------------+
//        | entry[0]   | entry[n]   | restart[0]   | restart[n]   | num_restarts    |
//        +------------+------------+--------------+--------------+-----------------+
        // 从文件尾部解析出 num_restarts
        uint32_t NumRestarts() const;

        const char *data_;
        size_t size_;
        uint32_t restart_offset_;     // Offset in data_ of restart array
        bool owned_;                  // 当前

        // No copying allowed
        Block(const Block &);
        void operator=(const Block &);

        class Iter;
    };

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_TABLE_BLOCK_H_
