#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/basic.hpp"
#include "utility.hpp"

namespace kun::algo
{
// rotate by complete system of residues
template<typename T, typename TS> KUN_INLINE void rotate(T p, TS count, TS amount)
{
    if (amount != 0)
    {
        TS loc_gcd = gcd(count, amount);

        // item count of a complete system of residues
        TS cycle_size = count / loc_gcd;

        // move per complete system of residues
        for (TS i = 0; i < loc_gcd; ++i)
        {
            auto buf_object = std::move(*(p + i));
            TS index_to_fill = i;
            // move per element of complete system of residues by amount
            for (TS j = 0; j < cycle_size; ++j)
            {
                index_to_fill = (index_to_fill + amount) % count;
                ::kun::swap(*(p + index_to_fill), buf_object);
            }
        }
    }
}
}// namespace kun::algo