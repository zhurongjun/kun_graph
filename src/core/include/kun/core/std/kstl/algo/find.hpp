#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "functor.hpp"

namespace kun::algo
{
template<typename T, typename TF, typename TP = Equal<>> KUN_INLINE T find(T begin, T end, TP&& p = TP())
{
    for (; begin != end; ++begin)
    {
        if (p(*begin))
            return begin;
    }
    return end;
}

template<typename T, typename TF, typename TP = Equal<>> KUN_INLINE T findLast(T begin, T end, TP&& p = TP())
{
    for (; begin != end; --end)
    {
        if (p(*end))
            return end;
    }
    return end;
}
}// namespace kun::algo
