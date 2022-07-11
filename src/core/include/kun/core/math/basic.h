#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/types.hpp"

namespace kun
{
// count zero, from low to high
// e.g. the memory layout of 12 in u32 is [0000 0000 0000 0000 0000 0000 0000 1100]
// then bitScan() will return 2, and bitScanR() will return 28(7 * 4)
KUN_CORE_API u64 bitScan(u64 v);
KUN_CORE_API u32 bitScan(u32 v);
KUN_CORE_API u64 bitScanR(u64 v);
KUN_CORE_API u32 bitScanR(u32 v);

// integer ceil floor round
template<class T> T divCeil(T a, T b);
template<class T> T divFloor(T a, T b);
template<class T> T divRound(T a, T b);

// ceil log
u32 ceilLog2(u32 v);
u64 ceilLog2(u64 v);

// ceil to power of 2
u32 ceilPowerOf2(u32 v);
u64 ceilPowerOf2(u64 v);
}// namespace kun

// impl
namespace kun
{
// integer ceil floor round
template<class T> KUN_INLINE T divCeil(T a, T b) { return (a + b - 1) / b; }
template<class T> KUN_INLINE T divFloor(T a, T b) { return a / b; }
template<class T> KUN_INLINE T divRound(T a, T b) { return (a >= 0) ? (a + b / 2) / b : (a - b / 2 + 1) / b; }

// ceil log
KUN_INLINE u32 ceilLog2(u32 v)
{
    u32 bit_mask = (bitScanR(v) << 26) >> 31;
    return (32 - bitScanR(v - 1)) & ~bit_mask;
}
KUN_INLINE u64 ceilLog2(u64 v)
{
    u64 bit_mask = (bitScanR(v) << 57llu) >> 63;
    return (64 - bitScanR(v - 1)) & ~bit_mask;
}

// ceil to power of 2
KUN_INLINE u32 ceilPowerOf2(u32 v) { return 1 << ceilLog2(v); }
KUN_INLINE u64 ceilPowerOf2(u64 v) { return 1 << ceilLog2(v); }
}// namespace kun