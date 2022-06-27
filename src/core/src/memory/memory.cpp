#include "kun/core/memory/memory.h"
#include "kun/core/functional/assert.hpp"
#include "kun/core/math/basic.h"
#include <mimalloc.h>

// malloc
namespace kun::memory
{
void free(void* p) { mi_free(p); }
void* malloc(Size n, Size alignment) { return mi_malloc_aligned(n, alignment); }
void* realloc(void* p, Size n, Size alignment) { return mi_realloc_aligned(p, n, alignment); }
void* malloc(Size n, Size alignment, Size offset) { return mi_malloc_aligned_at(n, alignment, offset); }
void* realloc(void* p, Size n, Size alignment, Size offset) { return mi_realloc_aligned_at(p, n, alignment, offset); }
}// namespace kun::memory

// memory ops
namespace kun::memory
{
void* memcpy(void* dst, const void* src, Size size) { return ::memcpy(dst, src, size); }
void* memmove(void* dst, const void* src, Size size) { return ::memmove(dst, src, size); }
i32 memcmp(const void* buf1, const void* buf2, size_t size) { return ::memcmp(buf1, buf2, size); }
void* memset(void* dst, u8 ch, Size size) { return ::memset(dst, ch, size); }
void* memzero(void* dst, Size size) { return ::memset(dst, 0, size); }
void bigMemswap(void* buf1, void* buf2, Size size)
{
    // check ptr
    KUN_Assertf(buf1 && buf2, "Pointers must be no-null: {}, {}", buf1, buf2);

    // check size
    KUN_Assert(size > 8);

    union PtrUnion
    {
        void* pVoid;
        uint8_t* p8;
        uint16_t* p16;
        uint32_t* p32;
        uint64_t* p64;
        Size pSize;
    };

    PtrUnion u1 = {buf1};
    PtrUnion u2 = {buf2};

    // make memory aligned
    if (u1.pSize & 1)
    {
        ::kun::swap(*u1.p8++, *u2.p8++);
        size -= 1;
    }
    if (u1.pSize & 2)
    {
        ::kun::swap(*u1.p16++, *u2.p16++);
        size -= 2;
    }
    if (u1.pSize & 4)
    {
        ::kun::swap(*u1.p32++, *u2.p32++);
        size -= 4;
    }

    // aligned fast swap
    u32 common_align = std::min(bitScan((u32)(u1.pSize - u2.pSize)), 3u);
    switch (common_align)
    {
        default:
            for (; size >= 8; size -= 8) { ::kun::swap(*u1.p64++, *u2.p64++); }
        case 2:
            for (; size >= 4; size -= 4) { ::kun::swap(*u1.p32++, *u2.p32++); }
        case 1:
            for (; size >= 2; size -= 2) { ::kun::swap(*u1.p16++, *u2.p16++); }
        case 0:
            for (; size >= 1; size -= 1) { ::kun::swap(*u1.p8++, *u2.p8++); }
    }
}

}// namespace kun::memory