#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

namespace kun::algo
{
template<typename T, typename TP = Less<>> KUN_INLINE bool isSorted(T begin, T end, TP p = TP())
{
    if (begin < end)
    {
        T next = begin + 1;
        while (next != end)
        {
            if (p(*next, *begin))
            {
                return false;
            }

            ++begin;
            ++next;
        }
    }
    return true;
}
}// namespace kun::algo