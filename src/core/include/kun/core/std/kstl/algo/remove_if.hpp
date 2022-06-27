#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

namespace kun::algo
{
template<typename T, typename TP> T removeIf(T begin, T end, TP&& p = TP())
{
    while (begin < end)
    {
        // skip false part in head
        while (!p(*begin))
        {
            ++begin;
            if (begin == end)
            {
                break;
            }
        }

        // skip true part in tail
        do {
            --end;
            if (begin == end)
            {
                break;
            }
        } while (p(*end));

        // move bad point
        *begin = std::move(end);
        ++begin;
        --end;
    }
    return begin;
}
template<typename T, typename TP> T removeIfStable(T begin, T end, TP&& p = TP())
{
    auto write = begin;

    // skip head
    while (write < end)
    {
        if (p(*write))
        {
            break;
        }

        ++write;
    }

    // read ptr
    auto read = write;
    ++read;

    // add item from read ptr to write ptr
    while (read < end)
    {
        // move
        if (!p(*read))
        {
            *write = std::move(*read);
            ++write;
        }

        ++read;
    }

    return write;
}

}// namespace kun::algo