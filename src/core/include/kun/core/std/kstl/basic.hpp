#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

namespace kun
{
template<typename T> KUN_INLINE void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

inline constexpr Size npos = (Size)-1;
}// namespace kun