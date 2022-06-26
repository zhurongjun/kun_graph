#pragma once
#include "kun/core/config.h"

namespace kun
{
template<typename T> KUN_INLINE void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
}// namespace kun