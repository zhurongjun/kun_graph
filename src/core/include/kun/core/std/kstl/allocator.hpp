#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

// alloc template
namespace kun
{
class __AllocTemplate
{
public:
    using SizeType = Size;

    // impl it
    KUN_INLINE SizeType freeRaw(void*& p, SizeType align);
    KUN_INLINE SizeType reserveRaw(void*& p, SizeType size, SizeType align);

    // helper
    template<typename T> KUN_INLINE SizeType free(T*& p) { return freeRaw(p); }
    template<typename T> KUN_INLINE SizeType reserve(T*& p, SizeType size) { return reserveRaw(p, size * sizeof(T), alignof(size)); }

    // inherent from DefaultChangePolicy
    KUN_INLINE SizeType getGrow(SizeType size, SizeType capacity);  // size > capacity, calc grow
    KUN_INLINE SizeType getShrink(SizeType size, SizeType capacity);// size < capacity, calc shrink
};
}// namespace kun