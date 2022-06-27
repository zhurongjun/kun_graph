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
}// namespace kun