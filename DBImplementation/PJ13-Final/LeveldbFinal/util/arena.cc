// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/arena.h"

namespace leveldb {

    static const int kBlockSize = 4096;

    Arena::Arena() : memory_usage_(0) {
        alloc_ptr_ = nullptr;  // First allocation will allocate a block
        alloc_bytes_remaining_ = 0;
    }

    Arena::~Arena() {
        for (size_t i = 0; i < blocks_.size(); i++) {
            delete[] blocks_[i];
        }
    }

    // 当上一个 block 空间不够时，调用该函数
    // 1. 如果需要的空间大小大于 4096/4 bytes，直接分配一块 bytes 大小的空间然后返回；
    // 2. 如果需要的空间较小，则分配一块 4096 的块，然后一点点地分配给调用对象。
    // 所以第2种情况可能调用 AllocateNewBlock() 函数
    char *Arena::AllocateFallback(size_t bytes) {
        if (bytes > kBlockSize / 4) {
            // Object is more than a quarter of our block size.  Allocate it separately
            // to avoid wasting too much space in leftover bytes.
            char *result = AllocateNewBlock(bytes);
            return result;
        }

        // We waste the remaining space in the current block.
        alloc_ptr_ = AllocateNewBlock(kBlockSize);
        alloc_bytes_remaining_ = kBlockSize;

        char *result = alloc_ptr_;
        alloc_ptr_ += bytes;
        alloc_bytes_remaining_ -= bytes;
        return result;
    }

    // 分配内存对齐的空间,
    // 同样可能会调用 AllocateFallback()
    char *Arena::AllocateAligned(size_t bytes) {
        const int align = (sizeof(void *) > 8) ? sizeof(void *) : 8;
        assert((align & (align - 1)) == 0);   // Pointer size should be a power of 2
        size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
        size_t slop = (current_mod == 0 ? 0 : align - current_mod);
        size_t needed = bytes + slop;
        char *result;
        if (needed <= alloc_bytes_remaining_) {
            result = alloc_ptr_ + slop;
            alloc_ptr_ += needed;
            alloc_bytes_remaining_ -= needed;
        } else {
            // AllocateFallback always returned aligned memory
            result = AllocateFallback(bytes);
        }
        assert((reinterpret_cast<uintptr_t>(result) & (align - 1)) == 0);
        return result;
    }

    // 分配一个 4096 大小的块，并加入到 blocks_
    char *Arena::AllocateNewBlock(size_t block_bytes) {
        char *result = new char[block_bytes];
        blocks_.push_back(result);
        memory_usage_.fetch_add(block_bytes + sizeof(char *),
                                std::memory_order_relaxed);
        return result;
    }

}  // namespace leveldb
