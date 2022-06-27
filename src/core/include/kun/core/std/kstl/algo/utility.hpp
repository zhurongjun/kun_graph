#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

// gcd & lcm
namespace kun::algo
{
// greatest common divisor(gcd)
template<typename T> KUN_INLINE T gcd(T a, T b)
{
    while (b != 0)
    {
        T tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

// lowest common multiple(lcm)
template<typename T> KUN_INLINE T lcm(T a, T b)
{
    T gcd_val = gcd(a, b);
    return a * b / gcd_val;
}
}// namespace kun::algo
