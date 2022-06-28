#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/math/basic.h"
#include "kun/core/functional/assert.hpp"

namespace kun::algo
{
// bit mask
inline constexpr u32 NumBitsPerDWORD = 32;
inline constexpr u32 NumBitsPerDWORDLogTwo = 5;
inline constexpr u32 PerDWORDMask = NumBitsPerDWORD - 1;
inline constexpr u32 DWORDEmptyMask = 0u;
inline constexpr u32 DWORDFullMask = ~DWORDEmptyMask;

// calc
template<typename TS> KUN_INLINE TS calcNumWords(TS num_bits)
{
    KUN_Assert(num_bits >= 0);
    return divCeil(num_bits, (TS)NumBitsPerDWORD);
}
template<typename TS> KUN_INLINE u32 firstWordMask(TS index)
{
    KUN_Assert(index >= 0);
    return DWORDFullMask << (index % NumBitsPerDWORD);
}
template<typename TS> KUN_INLINE u32 lastWordMask(TS num_bits)
{
    KUN_Assert(num_bits >= 0);
    const u32 unused_bits = (NumBitsPerDWORD - num_bits % NumBitsPerDWORD) % NumBitsPerDWORD;
    return DWORDFullMask >> unused_bits;
}

// set
template<typename TS> KUN_INLINE void setWords(u32* words, TS num_words, bool v)
{
    if (num_words > 8)
    {
        memory::memset(words, v ? 0xff : 0, num_words * sizeof(u32));
    }
    else
    {
        u32 word = v ? DWORDFullMask : DWORDEmptyMask;
        for (TS i = 0; i < num_words; ++i) { words[i] = words; }
    }
}
template<typename TS> KUN_INLINE void setBitRange(u32* data, TS index, TS num, bool v)
{
    if (num == 0)
        return;

    // calculate dword index and count
    TS dword_start_index = divFloor(index, NumBitsPerDWORD);
    TS dword_count = divCeil(index + num, NumBitsPerDWORD) - dword_start_index;

    // calculate mask
    u32 start_mask = firstWordMask(index);
    u32 end_mask = lastWordMask(index + num);

    u32* d = data + dword_start_index;
    if (v)
    {
        // set to true
        if (dword_count == 1)
        {
            *d |= start_mask & end_mask;
        }
        else
        {
            *d++ |= start_mask;
            dword_count -= 2;// exclude start and end
            while (dword_count != 0)
            {
                *d++ = DWORDFullMask;
                --dword_count;
            }
            *d |= end_mask;
        }
    }
    else
    {
        // set to false
        if (dword_count == 1)
        {
            *d &= ~(start_mask & end_mask);
        }
        else
        {
            *d++ &= ~start_mask;
            dword_count -= 2;// exclude start and end
            while (dword_count != 0)
            {
                *d++ = 0;
                --dword_count;
            }
            *d &= ~end_mask;
        }
    }
}

// find bit
template<typename TS> KUN_INLINE TS findBit(const u32* data, TS num, bool v)
{
    const TS dword_count = calcNumWords(num);
    TS dword_index = 0;

    // test to skip head
    const u32 test = v ? DWORDEmptyMask : DWORDFullMask;

    // skip head
    while (dword_index < dword_count && data[dword_index] == test) ++dword_index;

    // now find bit
    if (dword_index < dword_count)
    {
        // reserve for CountTrailingZeros
        const u32 bits = v ? (data[dword_index]) : ~(data[dword_index]);
        KUN_Assert(bits != 0);
        const TS lowest_bit_idx = bitScan(bits) + (dword_index << NumBitsPerDWORDLogTwo);

        if (lowest_bit_idx < num)
            return lowest_bit_idx;
    }
    return npos;
}
template<typename TS> KUN_INLINE TS findLastBit(const u32* data, TS num, bool v)
{
    if (num != 0)
    {
        // init data
        TS dword_index = calcNumWords(num) - 1;
        u32 mask = lastWordMask(num);
        const u32 test = v ? DWORDEmptyMask : DWORDFullMask;

        // skip tail
        if ((data[dword_index] & mask) == (test & mask))
        {
            --dword_index;
            mask = DWORDFullMask;
            while (dword_index >= 0 && data[dword_index] == test) --dword_index;
        }

        // now find bit index
        if (dword_index >= 0)
        {
            const u32 bits = (v ? data[dword_index] : ~data[dword_index]) & mask;
            KUN_Assert(bits != 0);
            u32 bit_index = (NumBitsPerDWORD - 1) - bitScanR(bits);
            return bit_index + (dword_index << NumBitsPerDWORDLogTwo);
        }
    }

    return npos;
}

// find and set
template<typename TS> KUN_INLINE TS findAndSetFirstZeroBit(u32* data, TS num, TS start_idx)
{
    const TS dword_count = calcNumWords(num);
    TS dword_index = divFloor(start_idx, (TS)NumBitsPerDWORD);

    // skip big non-zero word
    while (dword_index < dword_count && data[dword_index] == DWORDFullMask) { ++dword_index; }

    // now search bit
    if (dword_index < dword_count)
    {
        const u32 bits = ~(data[dword_index]);
        KUN_Assert(bits != 0);
        const u32 lowest_bit = (bits) & (-(i32)bits);
        const TS lowest_bit_index = bitScan(bits) + (dword_index << NumBitsPerDWORDLogTwo);
        if (lowest_bit_index < num)
        {
            data[dword_index] |= lowest_bit;
            return lowest_bit_index;
        }
    }

    return npos;
}
template<typename TS> KUN_INLINE TS FindAndSetLastZeroBit(u32* data, TS num)
{
    // get the correct mask for the last word
    u32 mask = lastWordMask(num);

    // iterate over the array until we see a word with a zero bit.
    TS dword_index = calcNumWords(num);
    while (dword_index > 0)
    {
        --dword_index;
        if ((data[dword_index] & mask) != mask)
        {
            break;
        }
        mask = DWORDFullMask;
    }

    // flip the bits, then we only need to find the first one bit -- easy.
    const u32 bits = ~data[dword_index] & mask;
    KUN_Assert(bits != 0);
    u32 bit_index = (NumBitsPerDWORD - 1) - bitScanR(bits);
    data[dword_index] |= 1u << bit_index;

    return bit_index + (dword_index << NumBitsPerDWORDLogTwo);
}

// get & set
template<typename TS> KUN_INLINE void setBit(u32* data, TS index, bool v)
{
    auto word_idx = index >> NumBitsPerDWORDLogTwo;
    u32 word = data[word_idx];
    u32 mask = 1 << (index & PerDWORDMask);
    data[word_idx] = v ? word | mask : word & ~mask;
}
template<typename TS> KUN_INLINE bool getBit(u32* data, TS index)
{
    auto word_idx = index >> NumBitsPerDWORDLogTwo;
    return (data[word_idx] & (1 << (index & PerDWORDMask)));
}
}// namespace kun::algo
