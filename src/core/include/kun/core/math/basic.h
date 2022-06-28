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

}// namespace kun

// impl
namespace kun
{
template<class T> T divCeil(T a, T b) { return (a + b - 1) / b; }
template<class T> T divFloor(T a, T b) { return a / b; }
template<class T> T divRound(T a, T b) { return (a >= 0) ? (a + b / 2) / b : (a - b / 2 + 1) / b; }
}// namespace kun