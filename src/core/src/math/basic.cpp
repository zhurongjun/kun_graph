#pragma once
#include "kun/core/math/basic.h"

#if KUN_COMPILER == KUN_COMPILER_MSVC
    #include <intrin0.h>
    #pragma intrinsic(_BitScanReverse)
    #pragma intrinsic(_BitScanForward)
#endif

namespace kun
{
u32 bitScan(u32 v)
{
    if (v == 0)
    {
        return 32;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;       // 0-based, where the LSB is 0 and MSB is 31
        _BitScanForward(&bit_index, v);// Scans from LSB to MSB
        return bit_index;
#else

#endif
    }
}
u64 bitScan(u64 v)
{
    if (v == 0)
    {
        return 64;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;         // 0-based, where the LSB is 0 and MSB is 31
        _BitScanForward64(&bit_index, v);// Scans from LSB to MSB
        return v;
#else

#endif
    }
}
u32 bitScanR(u32 v)
{
    if (v == 0)
    {
        return 32;
    }
    else
    {
#if KUN_COMPILER == KUN_COMPILER_MSVC
        unsigned long bit_index;       // 0-based, where the LSB is 0 and MSB is 31
        _BitScanReverse(&bit_index, v);// Scans from LSB to MSB
        return 31 - bit_index;
#else

#endif
    }
}
u64 bitScanR(u64 v)
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
#else

#endif
    }
}
}// namespace kun