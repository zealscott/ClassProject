// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// A filter block is stored near the end of a Table file.  It contains
// filters (e.g., bloom filters) for all data blocks in the table combined
// into a single filter block.

#ifndef STORAGE_LEVELDB_TABLE_FILTER_BLOCK_H_
#define STORAGE_LEVELDB_TABLE_FILTER_BLOCK_H_

#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "leveldb/slice.h"
#include "util/hash.h"

namespace leveldb {

    class FilterPolicy;

// A FilterBlockBuilder is used to construct all of the filters for a
// particular Table.  It generates a single string which is stored as
// a special block in the Table.
//
// The sequence of calls to FilterBlockBuilder must match the regexp:
//      (StartBlock AddKey*)* Finish
    class FilterBlockBuilder {
    public:
        explicit FilterBlockBuilder(const FilterPolicy *);

        void StartBlock(uint64_t block_offset);
        void AddKey(const Slice &key);
        Slice Finish();

    private:
        void GenerateFilter();

        const FilterPolicy *policy_;
        std::string keys_;              // Flattened key contents
        std::vector<size_t> start_;     // Starting index in keys_ of each key
        std::string result_;            // Filter data computed so far
        std::vector<Slice> tmp_keys_;   // policy_->CreateFilter() argument
        std::vector<uint32_t> filter_offsets_;

        // No copying allowed
        FilterBlockBuilder(const FilterBlockBuilder &);
        void operator=(const FilterBlockBuilder &);
    };


//    filter 格式和 class FilterBlockReader 成员变量指向关系如下:


//    data_
//    +---------------------+
//    | filter[1] : string  |
//    +---------------------+
//    +---------------------+
//    | filter[2] : string  |
//    +---------------------+
//    ...
//    +---------------------+
//    | filter[n] : string  |
//    +---------------------+
//    offset_
//    +----------------------+----------------------+-------+----------------------+-----------------------------------+---------------+
//    | offset[1] : uint32   | offset[2] : uint32   | ...   | offset[n] : uint32   | sum(sizeof(filter[i])) : uint32   | baselg : uint8|
//    +----------------------+----------------------+-------+----------------------+-----------------------------------+---------------+

//    上面的 filter[i] 是 filter data,
//          offset[i] 是 offset array
//    sum(sizeof(filter[i])) = offset_ - data_
    class FilterBlockReader {
    public:
        // REQUIRES: "contents" and *policy must stay live while *this is live.
        FilterBlockReader(const FilterPolicy *policy, const Slice &contents);
        bool KeyMayMatch(uint64_t block_offset, const Slice &key);

    private:
        const FilterPolicy *policy_;
        const char *data_;    // 指向 filter data
        const char *offset_;  // 指向 offset array
        size_t num_;          // offset array 大小，即上面的 n
        size_t base_lg_;      // Encoding parameter (see kFilterBaseLg in .cc file)
    };
}

#endif  // STORAGE_LEVELDB_TABLE_FILTER_BLOCK_H_
