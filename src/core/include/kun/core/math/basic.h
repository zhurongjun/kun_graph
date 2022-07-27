#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/types.hpp"

namespace kun
{
// count zero bit
// e.g. the memory layout of 12 in u32 is [0000 0000 0000 0000 0000 0000 0000 1100]
// then bitTailZero() will return 2, and bitLeadingZero() will return 28(7 * 4)
u64 bitTailZero(u64 v);
u32 bitTailZero(u32 v);
u64 bitLeadingZero(u64 v);
u32 bitLeadingZero(u32 v);

// value in highest/lowest bit
template<typename T> T highestBitValue(T v);
template<typename T> T lowestBitValue(T v);

// integer ceil floor round
template<typename T> T divCeil(T a, T b);
template<typename T> T divFloor(T a, T b);
template<typename T> T divRound(T a, T b);

// ceil log
u32 ceilLog2(u32 v);
u64 ceilLog2(u64 v);

// ceil to power of 2
u32 ceilPowerOf2(u32 v);
u64 ceilPowerOf2(u64 v);
}// namespace kun

#if KUN_COMPILER == KUN_COMPILER_MSVC
    #include <intrin0.h>
    #pragma intrinsic(_BitScanReverse)
    #pragma intrinsic(_BitScanForward)
    #pragma intrinsic(_BitScanReverse64)
    #pragma intrinsic(_BitScanForward64)
#endif

// impl bit count
namespace kun
{
KUN_INLINE u32 bitTailZero(u32 v)
{
    if (v == 0)
    {
        return 32;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;
        _BitScanForward(&bit_index, v);
        return bit_index;
#elif KUN_COMPILER == KUN_COMPILER_GCC
        int bit_index;
        bit_index = __builtin_ctzl(v);
        return bit_index;
#else
        // clang-format off

        // get lowest bit value
        v &= ~v + 1;

        // scan
        int bit_index = 31;
        if (v & 0x0000FFFF) bit_index -= 16;
        if (v & 0x00FF00FF) bit_index -= 8;
        if (v & 0x0F0F0F0F) bit_index -= 4;
        if (v & 0x33333333) bit_index -= 2;
        if (v & 0x55555555) bit_index -= 1;
        return bit_index;
        // clang-format on
#endif
    }
}
KUN_INLINE u64 bitTailZero(u64 v)
{
    if (v == 0)
    {
        return 64;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;
        _BitScanForward64(&bit_index, v);
        return v;
#elif KUN_COMPILER == KUN_COMPILER_GCC
        int bit_index;
        bit_index = __builtin_ctzll(v);
        return bit_index;
#else
        // clang-format off

        // get lowest bit value
        v &= ~v + 1;

        // scan
        int bit_index = 63;
        if (v & 0x00000000FFFFFFFF) bit_index -= 32;
        if (v & 0x0000FFFF0000FFFF) bit_index -= 16;
        if (v & 0x00FF00FF00FF00FF) bit_index -= 8;
        if (v & 0x0F0F0F0F0F0F0F0F) bit_index -= 4;
        if (v & 0x3333333333333333) bit_index -= 2;
        if (v & 0x5555555555555555) bit_index -= 1;
        return bit_index;
        // clang-format on
#endif
    }
}
KUN_INLINE u32 bitLeadingZero(u32 v)
{
    if (v == 0)
    {
        return 32;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;
        _BitScanReverse(&bit_index, v);
        return 31 - bit_index;
#elif KUN_COMPILER == KUN_COMPILER_GCC
        int bit_index;
        bit_index = (u32)__builtin_clzl(v);
        return bit_index;
#else
        // clang-format off

        // get highest bit value
        {
            u32 tmp = v;
            while (tmp)
            {
                v = tmp & (~tmp + 1);
                tmp &= ~v;
            }
        }

        // scan
        int bit_index = 31;
        if (v & 0xFFFF0000) bit_index -= 16;
        if (v & 0xFF00FF00) bit_index -= 8;
        if (v & 0xF0F0F0F0) bit_index -= 4;
        if (v & 0xCCCCCCCC) bit_index -= 2;
        if (v & 0xAAAAAAAA) bit_index -= 1;
        return bit_index;
        // clang-format on
#endif
    }
}
KUN_INLINE u64 bitLeadingZero(u64 v)
{
    if (v == 0)
    {
        return 64;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;
        _BitScanReverse64(&bit_index, v);
        return 63 - bit_index;
#elif KUN_COMPILER == KUN_COMPILER_GCC
        int bit_index;
        bit_index = (u32)__builtin_clzll(v);
        return bit_index;
#else
        // clang-format off

        // get highest bit value
        {
            u64 tmp = v;
            while (tmp)
            {
                v = tmp & (~tmp + 1);
                tmp &= ~v;
            }
        }

        // scan
        int bit_index = 63;
        if (v & 0xFFFFFFFF00000000) bit_index -= 32;
        if (v & 0xFFFF0000FFFF0000) bit_index -= 16;
        if (v & 0xFF00FF00FF00FF00) bit_index -= 8;
        if (v & 0xF0F0F0F0F0F0F0F0) bit_index -= 4;
        if (v & 0xCCCCCCCCCCCCCCCC) bit_index -= 2;
        if (v & 0xAAAAAAAAAAAAAAAA) bit_index -= 1;
        return bit_index;
        // clang-format on
#endif
    }
}
}// namespace kun

// impl others
namespace kun
{
// value in highest/lowest bit
template<typename T> KUN_INLINE T highestBitValue(T v)
{
    T result = 0;
    while (v)
    {
        result = lowestBitValue(v);
        v &= ~result;
    }
    return result;
}
template<typename T> KUN_INLINE T lowestBitValue(T v) { return v & (~v + 1); }

// integer ceil floor round
template<typename T> KUN_INLINE T divCeil(T a, T b) { return (a + b - 1) / b; }
template<typename T> KUN_INLINE T divFloor(T a, T b) { return a / b; }
template<typename T> KUN_INLINE T divRound(T a, T b) { return (a >= 0) ? (a + b / 2) / b : (a - b / 2 + 1) / b; }

// ceil log
KUN_INLINE u32 ceilLog2(u32 v)
{
    u32 bit_mask = (bitLeadingZero(v) << 26) >> 31;
    return (32 - bitLeadingZero(v - 1)) & ~bit_mask;
}
KUN_INLINE u64 ceilLog2(u64 v)
{
    u64 bit_mask = (bitLeadingZero(v) << 57llu) >> 63;
    return (64 - bitLeadingZero(v - 1)) & ~bit_mask;
}

// ceil to power of 2
KUN_INLINE u32 ceilPowerOf2(u32 v) { return 1 << ceilLog2(v); }
KUN_INLINE u64 ceilPowerOf2(u64 v) { return 1 << ceilLog2(v); }
}// namespace kun