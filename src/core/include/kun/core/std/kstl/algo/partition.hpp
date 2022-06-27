#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/basic.hpp"

namespace kun::algo
{
template<typename T, typename TP> T partition(T begin, T end, TP&& p = TP())
{
    while (begin < end)
    {
        // skip true part in head
        while (p(*begin))
        {
            ++begin;
            if (begin == end)
            {
                break;
            }
        }

        // skip false part in tail
        do {
            --end;
            if (begin == end)
            {
                break;
            }
        } while (!p(*end));

        // swap bad point
        ::kun::swap(*begin, *end);
        ++begin;
        --end;
    }
    return begin;
}
}// namespace kun::algo