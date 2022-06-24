#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/memory/memory.h"

namespace kun
{
class KUN_CORE_API EASTLAllocator
{
public:
    KUN_INLINE explicit EASTLAllocator(const char* = nullptr) {}
    KUN_INLINE EASTLAllocator(const EASTLAllocator&) {}
    KUN_INLINE EASTLAllocator(const EASTLAllocator&, const char*) {}

    KUN_INLINE EASTLAllocator& operator=(const EASTLAllocator&) { return *this; }

    KUN_INLINE void* allocate(size_t n, int flags = 0) { return malloc(n); }
    KUN_INLINE void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0) { return malloc(n, alignment, offset); }
    KUN_INLINE void deallocate(void* p, size_t n) { free(p); }

    KUN_INLINE const char* get_name() const { return ""; }
    KUN_INLINE void set_name(const char*) {}
};
KUN_INLINE bool operator==(const EASTLAllocator&, const EASTLAllocator&) { return true; }
KUN_INLINE bool operator!=(const EASTLAllocator&, const EASTLAllocator&) { return false; }

KUN_CORE_API EASTLAllocator* eastlDefaultAllocator();
}// namespace kun
