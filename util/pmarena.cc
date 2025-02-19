// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "util/pmarena.h"
#include <iostream>

namespace leveldb {

static const int kBlockSize = 4096;

PMarena::PMarena()
    : alloc_ptr_(nullptr), alloc_bytes_remaining_(0), memory_usage_(0) {}

PMarena::PMarena(PMmanager* pm_)
    :pm_manager(pm_), alloc_ptr_(nullptr), alloc_bytes_remaining_(0), memory_usage_(0) {}

PMarena::~PMarena() {
  for (size_t i = 0; i < blocks_.size(); i++) {
      pm_manager->Free(blocks_[i]);
  }
}

char* PMarena::AllocateFallback(size_t bytes) {
  if (bytes > kBlockSize / 4) {
    // Object is more than a quarter of our block size.  Allocate it separately
    // to avoid wasting too much space in leftover bytes.
    char* result = AllocateNewBlock(bytes);
    return result;
  }

  // We waste the remaining space in the current block.
  alloc_ptr_ = AllocateNewBlock(kBlockSize);
  alloc_bytes_remaining_ = kBlockSize;

  char* result = alloc_ptr_;
  alloc_ptr_ += bytes;
  alloc_bytes_remaining_ -= bytes;
  return result;
}

char* PMarena::AllocateAligned(size_t bytes) {
  const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
  static_assert((align & (align - 1)) == 0,
                "Pointer size should be a power of 2");
  size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
  size_t slop = (current_mod == 0 ? 0 : align - current_mod);
  size_t needed = bytes + slop;
  char* result;
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

char* PMarena::AllocateNewBlock(size_t block_bytes) {
  char* result = (char*)pm_manager->Allocate(block_bytes);
  //char* result = new char[block_bytes];
  blocks_.push_back(result);
  memory_usage_.fetch_add(block_bytes + sizeof(char*),
                          std::memory_order_relaxed);
  return result;
}

void PMarena::Free(void* ptr) {
  if (ptr == NULL) {
        return; // 如果 ptr 為 NULL，則無需執行任何操作
    }

    PMEMoid oid = pmemobj_oid(ptr);
    if (!OID_IS_NULL(oid)) {
        pmemobj_free(&oid);
    } else {
        perror("pmemobj_free error");
    }
}

}  // namespace leveldb
