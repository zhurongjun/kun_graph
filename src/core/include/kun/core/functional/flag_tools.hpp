#pragma once
#include "kun/core/config.h"

namespace kun
{
template<typename T> KUN_INLINE bool allFlag(T val, T flags) noexcept { return (val & flags) == flags; }
template<typename T> KUN_INLINE bool anyFlag(T val, T flags) noexcept { return val & flags; }
template<typename T> KUN_INLINE T setFlag(T val, T flags) noexcept { return val | flags; }
template<typename T> KUN_INLINE T eraseFlag(T val, T flags) noexcept { return val & (~flags); }
}// namespace kun
