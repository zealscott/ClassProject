// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_ARENA_H_
#define STORAGE_LEVELDB_UTIL_ARENA_H_

#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace leveldb {

    class Arena {
    public:
        Arena();

        ~Arena();

        // Return a pointer to a newly allocated memory block of "bytes" bytes.
        // 给定 bytes，返回对应大小的内存空间
        // 若当前 block 空间不够，调用 AllocateFallback()
        char *Allocate(size_t bytes);

        // Allocate memory with the normal alignment guarantees provided by malloc.
        // 分配内存对齐的空间,
        // 同样可能会调用 AllocateFallback()
        char *AllocateAligned(size_t bytes);

        // Returns an estimate of the total memory usage of data allocated
        // by the arena.
        size_t MemoryUsage() const {
            return memory_usage_.load(std::memory_order_relaxed);
        }

    private:
        // 当上一个 block 空间不够时，调用该函数
        // 1. 如果需要的空间大小大于 4096/4 bytes，直接分配一块 bytes 大小的空间然后返回；
        // 2. 如果需要的空间较小，则分配一块 4096 的块，然后一点点地分配给调用对象。
        // 所以第2种情况可能调用 AllocateNewBlock() 函数
        char *AllocateFallback(size_t bytes);
        // 分配一个 4096 大小的块，并加入到 blocks_
        char *AllocateNewBlock(size_t block_bytes);

        // Allocation state
        char *alloc_ptr_;
        size_t alloc_bytes_remaining_;

        // Array of new[] allocated memory blocks
        std::vector<char *> blocks_;

        // Total memory usage of the arena.
        //
        // TODO(costan): This member is accessed via atomics, but the others are
        //               accessed without any locking. Is this OK?
        std::atomic<size_t> memory_usage_;

        // No copying allowed
        Arena(const Arena &);

        void operator=(const Arena &);
    };

    // 给定 bytes，返回对应大小的内存空间
    // 若当前 block 空间不够，调用 AllocateFallback()
    inline char *Arena::Allocate(size_t bytes) {
        // The semantics of what to return are a bit messy if we allow
        // 0-byte allocations, so we disallow them here (we don't need
        // them for our internal use).
        assert(bytes > 0);
        if (bytes <= alloc_bytes_remaining_) {
            char *result = alloc_ptr_;
            alloc_ptr_ += bytes;
            alloc_bytes_remaining_ -= bytes;
            return result;
        }
        return AllocateFallback(bytes);
    }

}  // namespace leveldb

#endif  // STORAGE_LEVELDB_UTIL_ARENA_H_
