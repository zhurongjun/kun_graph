#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/basic.hpp"

// malloc
namespace kun::memory
{
// alignment
enum
{
    DEFAULT_ALIGNMENT = 4
};

// malloc
KUN_CORE_API void free(void* p);
KUN_CORE_API void* malloc(Size n, Size alignment = DEFAULT_ALIGNMENT);
KUN_CORE_API void* realloc(void* p, Size n, Size alignment = DEFAULT_ALIGNMENT);
KUN_CORE_API void* malloc(Size n, Size alignment, Size offset);
KUN_CORE_API void* realloc(void* p, Size n, Size alignment, Size offset);
}// namespace kun::memory

// memory ops
namespace kun::memory
{
// memory ops
KUN_CORE_API void* memcpy(void* dst, const void* src, Size size);
KUN_CORE_API void* memmove(void* dst, const void* src, Size size);
KUN_CORE_API i32 memcmp(const void* buf1, const void* buf2, size_t size);
KUN_CORE_API void* memset(void* dst, u8 ch, Size size);
KUN_CORE_API void* memzero(void* dst, Size size);
KUN_CORE_API void bigMemswap(void* buf1, void* buf2, Size size);
KUN_INLINE void memswap(void* buf1, void* buf2, Size size)
{
    switch (size)
    {
        case 0: break;
        case 1: ::kun::swap(*(u8*)buf1, *(u8*)buf2); break;
        case 2: ::kun::swap(*(u16*)buf1, *(u16*)buf2); break;
        case 3:
            ::kun::swap(*((u16*&)buf1)++, *((u16*&)buf2)++);
            ::kun::swap(*(u8*)buf1, *(u8*)buf2);
            break;
        case 4: ::kun::swap(*(u32*)buf1, *(u32*)buf2); break;
        case 5:
            ::kun::swap(*((u32*&)buf1)++, *((u32*&)buf2)++);
            ::kun::swap(*(u8*)buf1, *(u8*)buf2);
            break;
        case 6:
            ::kun::swap(*((u32*&)buf1)++, *((u32*&)buf2)++);
            ::kun::swap(*(u16*)buf1, *(u16*)buf2);
            break;
        case 7:
            ::kun::swap(*((u32*&)buf1)++, *((u32*&)buf2)++);
            ::kun::swap(*((u16*&)buf1)++, *((u16*&)buf2)++);
            ::kun::swap(*(u8*)buf1, *(u8*)buf2);
            break;
        case 8: ::kun::swap(*(u64*)buf1, *(u64*)buf2); break;
        case 16:
            ::kun::swap(((u64*)buf1)[0], ((u64*)buf2)[0]);
            ::kun::swap(((u64*)buf1)[1], ((u64*)buf2)[1]);
            break;
        default: bigMemswap(buf1, buf2, size); break;
    }
}
}// namespace kun::memory