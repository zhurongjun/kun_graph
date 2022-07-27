#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/algo/bit_array.hpp"
#include "fwd.hpp"

// bit ref
namespace kun
{
class BitRef
{
public:
    KUN_INLINE BitRef(u32& data, u32 mask)
        : m_data(data)
        , m_mask(mask)
    {
    }
    KUN_INLINE         operator bool() const { return (m_data & m_mask); }
    KUN_INLINE BitRef& operator=(bool v)
    {
        m_data = v ? m_data | m_mask : m_data & ~m_mask;
        return *this;
    }
    KUN_INLINE BitRef& operator=(const BitRef& rhs)
    {
        *this = (bool)rhs;
        return *this;
    }

private:
    u32& m_data;
    u32  m_mask;
};
}// namespace kun

// bit iterator
namespace kun
{
template<typename TS, bool Const> struct BitIt
{
public:
    template<typename TAlloc> using BitArrayType = std::conditional_t<Const, const BitArray<TAlloc>, BitArray<TAlloc>>;
    using DataPtr = std::conditional_t<Const, const u32*, u32*>;
    using RefType = std::conditional_t<Const, bool, BitRef>;

    KUN_INLINE BitIt(DataPtr data, TS size, TS start = 0)
        : m_data(data)
        , m_size(size)
        , m_bit_index(start)
        , m_DWORD_index(start >> algo::NumBitsPerDWORDLogTwo)
        , m_mask(1 << (start & algo::PerDWORDMask))
    {
    }
    template<typename TAlloc>
    KUN_INLINE explicit BitIt(BitArray<TAlloc>& arr, TS start = 0)
        : BitIt(arr.data(), arr.size(), start)
    {
    }

    KUN_INLINE BitIt& operator++()
    {
        ++m_bit_index;
        m_mask <<= 1;

        // advance to the next uint32.
        if (!m_mask)
        {
            m_mask = 1;
            ++m_DWORD_index;
        }
        return *this;
    }
    KUN_INLINE explicit operator bool() const { return m_bit_index < m_size; }
    KUN_INLINE bool     operator!() const { return !(bool)*this; }
    KUN_INLINE bool     operator==(const BitIt& rhs) const { return m_data == rhs.m_data && m_bit_index == rhs.m_bit_index; }
    KUN_INLINE bool     operator!=(const BitIt& rhs) const { return !(*this == rhs); }
    KUN_INLINE RefType  operator*() const { return value(); }

    KUN_INLINE RefType value() const
    {
        if constexpr (Const)
        {
            return m_data[m_DWORD_index] & m_mask;
        }
        else
        {
            return RefType(m_data[m_DWORD_index], m_mask);
        }
    }
    KUN_INLINE TS index() const { return m_bit_index; }

private:
    DataPtr m_data;       // data ptr
    TS      m_size;       // data size(in bit)
    TS      m_bit_index;  // current bit index
    TS      m_DWORD_index;// current DWORD index
    u32     m_mask;       // current bit mask
};
}// namespace kun

// true bit iterator
namespace kun
{
template<typename TS> class TrueBitIt
{
public:
    KUN_INLINE TrueBitIt(const u32* data, TS size, TS start = 0)
        : m_data(data)
        , m_size(size)
        , m_bit_index(start)
        , m_DWORD_index(start >> algo::NumBitsPerDWORDLogTwo)
        , m_eval_mask(1 << (start & algo::PerDWORDMask))
        , m_step_mask(algo::DWORDFullMask << (start & algo::PerDWORDMask))
    {
        KUN_Assert(start >= 0 && start <= size);
        _findFirstSetBit();
    }
    template<typename TAlloc>
    KUN_INLINE TrueBitIt(const BitArray<TAlloc>& arr, TS start = 0)
        : TrueBitIt(arr.data(), arr.size(), start)
    {
    }

    KUN_INLINE TrueBitIt& operator++()
    {
        // skip current mask
        m_step_mask &= ~m_eval_mask;

        // find next mask
        _findFirstSetBit();
        return *this;
    }

    KUN_INLINE bool     operator==(const TrueBitIt& rhs) const { return m_bit_index == rhs.m_bit_index && m_data == rhs.m_data; }
    KUN_INLINE bool     operator!=(const TrueBitIt& rhs) const { return !(*this == rhs); }
    KUN_INLINE explicit operator bool() const { return m_bit_index < m_size; }
    KUN_INLINE bool     operator!() const { return !(bool)*this; }
    KUN_INLINE TS       index() const { return m_bit_index; }

private:
    KUN_INLINE void _findFirstSetBit()
    {
        const TS last_DWORD_idx = (m_size - 1) / algo::NumBitsPerDWORD;

        // skip zero words
        u32 val = m_data[m_DWORD_index] & m_step_mask;
        while (!val)
        {
            ++m_DWORD_index;

            // out of bound
            if (m_DWORD_index > last_DWORD_idx)
            {
                m_bit_index = m_size;
                return;
            }

            val = m_data[m_DWORD_index];
            m_step_mask = algo::DWORDFullMask;
        }

        // get bit mask
        const u32 newVal = val & (val - 1);
        m_eval_mask = newVal ^ val;

        // if the Nth bit was the lowest set bit of BitMask, then this gives us N
        m_bit_index = m_DWORD_index * algo::NumBitsPerDWORD + bitTailZero(m_eval_mask);

        // out of bound
        if (m_bit_index > m_size)
            m_bit_index = m_size;
    }

private:
    const u32* m_data;       // data ptr
    TS         m_size;       // data size(in bit)
    TS         m_bit_index;  // current bit index
    TS         m_DWORD_index;// current DWORD index
    u32        m_eval_mask;  // bit mask used to eval bit value
    u32        m_step_mask;  // bit mask used to recorde current step
};
}// namespace kun