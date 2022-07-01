#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/memory/copy_move_policy.hpp"
#include "kun/core/functional/assert.hpp"

namespace kun::algo
{
template<typename T, typename TP> KUN_INLINE T removeIf(T begin, T end, TP&& p = TP())
{
    if (begin < end)
    {
        auto write = begin;
        auto read = begin;
        bool do_remove = !p(*read);

        do {
            auto run_start = read;
            ++read;

            // collect run scope
            while (read < end && do_remove == p(*read)) { ++read; }
            Size run_len = read - run_start;
            KUN_Assert(run_len > 0);

            // do scope op
            if (do_remove)
            {
                // destruct items
                ::kun::memory::destructItem(run_start, run_len);
            }
            else
            {
                // move item
                if (write != begin)
                {
                    // copy items
                    ::kun::memory::copyItems(write, run_start, run_len);
                }
                write += run_len;
            }

            // update flag
            do_remove = !do_remove;
        } while (read < end);
        return write;
    }
    return end;
}
template<typename T, typename TP> KUN_INLINE T removeIfSwap(T begin, T end, TP&& p = TP())
{
    while (begin < end)
    {
        // skip items that needn't remove on header
        while (begin < end)
        {
            if (p(*begin))
            {
                break;
            }
            ++begin;
        }

        // skip items that need remove on tail
        while (begin < end)
        {
            if (!p(*end))
            {
                break;
            }
            --end;
        }

        // swap
        ::kun::swap(*begin, *end);

        // update iterator
        ++begin;
        --end;
    }
    return begin;
}
}// namespace kun::algo