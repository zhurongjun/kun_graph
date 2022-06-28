#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/basic.hpp"
#include "binarySearch.hpp"

// merge sort help
namespace kun::algo::__help
{
template<class T, class TP> void merge(T begin, T mid, T end, TP&& p)
{
    T l_start = begin;
    T r_start = mid;

    while (l_start < r_start && r_start < end)
    {
        // find insert location for first element of right part head in left part
        T new_l_start = upperBound(l_start, r_start, *r_start, std::forward<TP>(p));
        l_start = new_l_start;

        // check done
        if (l_start >= r_start)
            break;

        // find insert location for first element of updated left part head in right part
        // then r_start <---> new_r_start can be rotated to l_start, and l_start <---> r_start can be rotated to r_start
        // and then l_start <---> moved new_r_start will be sorted
        // here is example
        // init state, left part is '.', right part is ','                                       |..........|,,,,,,,,,|
        // step.1 update left part, all '.' is less than any  ','                                |....LLLLLL|,,,,,,,,,|
        // step.2 update right part, all 'R' is less than any 'L'                                |....LLLLLL|RRRRR,,,,|
        // step.3 rotate, new bound is after 'R', because any 'R' is less than 'L' and ','       |....RRRRR|LLLLLL,,,,|
        T new_r_start = lowerBound(r_start, end, *l_start, std::forward<TP>(p));
        Size r_offset = new_r_start - r_start;

        // rotate mid part
        rotate(l_start, (Size)(new_r_start - l_start), r_offset);

        // now left bound is sorted, we can skip it
        l_start += r_offset + 1;
        r_start = new_r_start;
    }
}
}// namespace kun::algo::__help

// merge sort impl
namespace kun::algo
{
template<typename T, typename TP = Less<>, int MinMergeSubgroupSize = 2> void mergeSort(T begin, T end, TP&& p = TP())
{
    Size subgroup_start = 0;
    Size count = end - begin;

    // step1. sort per group
    if constexpr (MinMergeSubgroupSize > 1)
    {
        if constexpr (MinMergeSubgroupSize > 2)
        {
            // if group size > 3, we use simple bubble-sort.
            do {
                Size group_end = std::min(subgroup_start + MinMergeSubgroupSize, count);
                do {
                    for (Size it = subgroup_start; it < group_end - 1; ++it)
                    {
                        if (p(*(begin + (it + 1)), *(begin + it)))
                        {
                            ::kun::swap(*(begin + it), *(begin + (it + 1)));
                        }
                    }
                    --group_end;
                } while (group_end - subgroup_start > 1);

                subgroup_start += MinMergeSubgroupSize;
            } while (subgroup_start < count);
        }
        else
        {
            for (Size subgroup = 0; subgroup < count; subgroup += 2)
            {
                if ((subgroup + 1) < count && p(*(begin + (subgroup + 1)), *(begin + subgroup)))
                {
                    ::kun::swap(*(begin + subgroup), *(begin + (subgroup + 1)));
                }
            }
        }
    }

    // step2. merge groups
    Size subgroup_size = MinMergeSubgroupSize;
    while (subgroup_size < count)
    {
        auto next_group_size = subgroup_size << 1;
        subgroup_start = 0;
        do {
            auto cur_start = begin + subgroup_start;
            __help::merge(cur_start, cur_start + subgroup_size, cur_start + std::min(next_group_size, count - subgroup_start), std::forward<TP>(p));
            subgroup_start += next_group_size;
        } while (subgroup_start < count);

        subgroup_size = next_group_size;
    }
}
}// namespace kun::algo