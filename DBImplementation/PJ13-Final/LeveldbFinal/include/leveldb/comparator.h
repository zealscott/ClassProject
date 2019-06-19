// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_INCLUDE_COMPARATOR_H_
#define STORAGE_LEVELDB_INCLUDE_COMPARATOR_H_

#include <string>
#include "leveldb/export.h"

namespace leveldb {

    class Slice;

// A Comparator object provides a total order across slices that are
// used as keys in an sstable or a database.  A Comparator implementation
// must be thread-safe since leveldb may invoke its methods concurrently
// from multiple threads.
    class LEVELDB_EXPORT Comparator {
    public:
        virtual ~Comparator();

        // Three-way comparison.  Returns value:
        //   < 0 iff "a" < "b",
        //   == 0 iff "a" == "b",
        //   > 0 iff "a" > "b"
        virtual int Compare(const Slice &a, const Slice &b) const = 0;

        // The name of the comparator.  Used to check for comparator
        // mismatches (i.e., a DB created with one comparator is
        // accessed using a different comparator.
        //
        // The client of this package should switch to a new name whenever
        // the comparator implementation changes in a way that will cause
        // the relative ordering of any two keys to change.
        //
        // Names starting with "leveldb." are reserved and should not be used
        // by any clients of this package.
        virtual const char *Name() const = 0;

        // Advanced functions: these are used to reduce the space requirements
        // for internal data structures like index blocks.

        // If *start < limit, changes *start to a short string in [start,limit).
        // Simple comparator implementations may return with *start unchanged,
        // i.e., an implementation of this method that does nothing is correct.

        // 最原始的实现在 comparator.cc 中
        // 这个函数主要用在 TableBuilder::Add() 函数中调用

        // 什么作用呢？
        // 举个例子：
        // 1. 假设传进来两个 key: "the who" 和 "the whose"
        //    那么该函数会将 start 的值改为 “the who"
        // 2. 假设传进来两个 key: "the quick brown fox" 和 "the who"
        //    那么该函数会将 start 的值改为 “the r"
        // 3. 假设传进来两个 key: "the quick brown fox" 和 "the reason"
        //    那么该函数会将 start 的值改为 “the q"

        // 这么做的目的是什么呢？ 让 start 字符串长度尽可能的短，且不能小于原始的 start 值
        virtual void FindShortestSeparator(
                std::string *start,
                const Slice &limit) const = 0;

        // Changes *key to a short string >= *key.
        // Simple comparator implementations may return with *key unchanged,
        // i.e., an implementation of this method that does nothing is correct.
        virtual void FindShortSuccessor(std::string *key) const = 0;
    };

// Return a builtin comparator that uses lexicographic byte-wise
// ordering.  The result remains the property of this module and
// must not be deleted.
    LEVELDB_EXPORT const Comparator *BytewiseComparator();
    LEVELDB_EXPORT const Comparator *MyComparator();

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_INCLUDE_COMPARATOR_H_
