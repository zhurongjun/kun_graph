#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

namespace kun::algo
{
template<typename T, typename TF, typename TM = MapFwd, typename TP = Equal<>>
KUN_INLINE T find(T begin, T end, const TF& v, TM&& m = TM(), TP&& p = TP())
{
    for (; begin != end; ++begin)
    {
        if (p(m(*begin) == v))
            return begin;
    }
    return end;
}

template<typename T, typename TF, typename TM = MapFwd, typename TP = Equal<>>
KUN_INLINE T findLast(T begin, T end, const TF& v, TM&& m = TM(), TP&& p = TP())
{
    for (; begin != end; --end)
    {
        if (p(m(*end) == v))
            return end;
    }
    return end;
}
}// namespace kun::algo
