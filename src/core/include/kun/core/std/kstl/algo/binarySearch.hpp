#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

// binary search
namespace kun::algo
{
// find first value that >= target
template<typename T, typename TF, typename TP = Less<>> KUN_INLINE T lowerBound(T begin, T end, const TF& v, TP p = TP())
{
    while (end > begin)
    {
        const Size size = (end - begin);
        const Size left_over_size = size % 2;
        auto middle = begin + (size / 2);

        bool pass_check = p(*middle, v);

        begin = pass_check ? middle + left_over_size : begin;
        end = pass_check ? end : middle;
    }
    return begin;
}

// find first value thar > target
template<typename T, typename TF, typename TP = Less<>> KUN_INLINE T upperBound(T begin, T end, const TF& v, TP p = TP())
{
    while (end > begin)
    {
        const Size size = (end - begin);
        const Size left_over_size = size % 2;
        auto middle = begin + (size / 2);

        bool pass_check = !p(v, *middle);

        begin = pass_check ? middle + left_over_size : begin;
        end = pass_check ? end : middle;
    }
    return begin;
}
template<typename T, typename TF, typename TP = Less<>> KUN_INLINE T binarySearch(T begin, T end, const TF& v, TP p = TP())
{
    auto check_item = lowerBound(begin, end, v, p);
    if (check_item < end)
    {
        if (!p(v, *check_item))
        {
            return check_item;
        }
    }
    return end;
}
}// namespace kun::algo