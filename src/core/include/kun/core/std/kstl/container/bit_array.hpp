#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/algo/bit_array.hpp"
#include "kun/core/memory/memory.h"
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
template<typename TS, bool Const> class BitIt
{
    using DataPtr = std::conditional_t<Const, const u32*, u32*>;
    using Ref = std::conditional_t<Const, bool, BitRef>;
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
    KUN_INLINE Ref      operator*() const { return value(); }

    KUN_INLINE Ref value() const
    {
        if constexpr (Const)
        {
            return m_data[m_DWORD_index] & m_mask;
        }
        else
        {
            return Ref(m_data[m_DWORD_index], m_mask);
        }
    }
    KUN_INLINE TS index() const { return m_bit_index; }

private:
    DataPtr* m_data;       // data ptr
    TS       m_size;       // data size(in bit)
    TS       m_bit_index;  // current bit index
    TS       m_DWORD_index;// current DWORD index
    u32      m_mask;       // current bit mask
};
}// namespace kun

// bit array def
namespace kun
{
template<typename Alloc> class BitArray final
{
public:
    using SizeType = typename Alloc::SizeType;

    // ctor & dtor
    BitArray(Alloc alloc = Alloc());
    BitArray(bool v, SizeType size, Alloc alloc = Alloc());
    ~BitArray();

    // copy & move ctor
    BitArray(const BitArray& other, Alloc alloc = Alloc());
    BitArray(BitArray&& other);

    // copy & move assign
    BitArray& operator=(const BitArray& rhs);
    BitArray& operator=(BitArray&& rhs);

    // compare
    bool operator==(const BitArray& rhs) const;
    bool operator!=(const BitArray& rhs) const;
    bool operator<(const BitArray& rhs) const;

    // getter
    u32*         data();
    const u32*   data() const;
    SizeType     size() const;
    SizeType     capacity() const;
    Alloc&       allocator();
    const Alloc& allocator() const;
    bool         empty();

    // operator []
    BitRef operator[](SizeType idx);
    bool   operator[](SizeType idx) const;

    // memory op
    void clear();
    void release(SizeType capacity = 0);
    void reserve(SizeType capacity);
    void resize(SizeType size, bool new_value);
    void resizeUnsafe(SizeType size);

    // push
    SizeType add(bool v);
    SizeType add(bool v, SizeType n);

    // remove
    void removeAt(SizeType start, SizeType n = 1);
    void removeAtSwap(SizeType start, SizeType n = 1);

    // find
    SizeType find(bool v) const;
    SizeType findLast(bool v) const;

    // find and set
    SizeType findAndSetFirstZero(SizeType start);
    SizeType findAndSetLastZero();

    // contain
    bool contain(bool v) const;

    // set range
    void setRange(SizeType idx, SizeType n, bool v);

    // validate
    bool isValidIndex(SizeType idx);

    // support foreach
    BitIt<SizeType, false> begin();
    BitIt<SizeType, false> end();
    BitIt<SizeType, true>  begin() const;
    BitIt<SizeType, true>  end() const;

private:
    // helper
    void _resizeGrow();
    void _resizeTo(SizeType size);

private:
    u32*     m_data;
    SizeType m_size;
    SizeType m_capacity;
    Alloc    m_allocator;
};
}// namespace kun

// bit array impl
namespace kun
{
// helper
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::_resizeGrow()
{
    if (m_size > m_capacity)
    {
        SizeType word_size = algo::calcNumWords(m_size);
        SizeType word_capacity = algo::calcNumWords(m_capacity);
        word_capacity = m_allocator.getGrow(word_size, word_capacity);
        m_capacity = m_allocator.reserve(m_data, word_capacity) << algo::NumBitsPerDWORDLogTwo;
    }
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::_resizeTo(SizeType size)
{
    SizeType old_word_capacity = algo::calcNumWords(m_capacity);
    SizeType new_word_capacity = algo::calcNumWords(size);

    if (new_word_capacity)
    {
        // realloc
        new_word_capacity = m_allocator.reserve(m_data, new_word_capacity);

        // clean new memory
        if (new_word_capacity > old_word_capacity)
            memory::memzero(data() + old_word_capacity, (new_word_capacity - old_word_capacity) * sizeof(u32));

        // change max
        m_capacity = new_word_capacity * algo::NumBitsPerDWORD;
    }
    else
    {
        // free
        m_capacity = m_allocator.free(m_data);
        m_size = 0;
    }
}

// ctor & dtor
template<typename Alloc>
KUN_INLINE BitArray<Alloc>::BitArray(Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_allocator(std::move(alloc))
{
}
template<typename Alloc>
KUN_INLINE BitArray<Alloc>::BitArray(bool v, SizeType size, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_allocator(std::move(alloc))
{
    resize(size, v);
}
template<typename Alloc> KUN_INLINE BitArray<Alloc>::~BitArray() { release(); }

// copy & move ctor
template<typename Alloc>
KUN_INLINE BitArray<Alloc>::BitArray(const BitArray& other, Alloc alloc)
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_allocator(std::move(alloc))
{
    // resize
    _resizeTo(other.m_capacity);

    // copy
    m_size = other.size();
    if (other.m_size)
        memory::memcpy(m_data, other.m_data, algo::calcNumWords(m_size) * sizeof(u32));
}
template<typename Alloc>
KUN_INLINE BitArray<Alloc>::BitArray(BitArray&& other)
    : m_data(other.m_data)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity)
    , m_allocator(std::move(other.m_allocator))
{
}

// copy & move assign
template<typename Alloc> KUN_INLINE BitArray<Alloc>& BitArray<Alloc>::operator=(const BitArray& rhs)
{
    if (this == &rhs)
        return *this;

    // copy data
    m_data = rhs.m_data;
    m_size = rhs.m_size;
    m_capacity = rhs.m_capacity;
    m_allocator = std::move(rhs.m_allocator);

    // invalidate rhs
    rhs.m_data = nullptr;
    rhs.m_size = 0;
    rhs.m_capacity = 0;
    return *this;
}
template<typename Alloc> KUN_INLINE BitArray<Alloc>& BitArray<Alloc>::operator=(BitArray&& rhs)
{
    if (this == &rhs)
        return *this;

    _resizeTo(rhs.m_size);
    m_size = rhs.m_size;
    if (m_size)
        memory::memcpy(m_data, rhs.m_data, algo::calcNumWords(m_size) * sizeof(u32));
    return *this;
}

// compare
template<typename Alloc> KUN_INLINE bool BitArray<Alloc>::operator==(const BitArray& rhs) const
{
    if (m_size != rhs.m_size)
        return false;
    return memory::memcmp(m_data, rhs.m_data, algo::calcNumWords(m_size) * sizeof(u32));
}
template<typename Alloc> KUN_INLINE bool BitArray<Alloc>::operator!=(const BitArray& rhs) const { return !(*this == rhs); }
template<typename Alloc> KUN_INLINE bool BitArray<Alloc>::operator<(const BitArray& rhs) const
{
    if (m_size != rhs.m_size)
        return m_size < rhs.m_size;

    SizeType num_words = algo::calcNumWords(m_size);

    for (SizeType i = 0; i < num_words; i++)
    {
        if (m_data[i] != rhs.m_data[i])
        {
            return m_data[i] < rhs.m_data[i];
        }
    }
    return false;
}

// getter
template<typename Alloc> KUN_INLINE u32*                               BitArray<Alloc>::data() { return m_data; }
template<typename Alloc> KUN_INLINE const u32*                         BitArray<Alloc>::data() const { return m_data; }
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::SizeType BitArray<Alloc>::size() const { return m_size; }
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::SizeType BitArray<Alloc>::capacity() const { return m_capacity; }
template<typename Alloc> KUN_INLINE Alloc&                             BitArray<Alloc>::allocator() { return m_allocator; }
template<typename Alloc> KUN_INLINE const Alloc&                       BitArray<Alloc>::allocator() const { return m_allocator; }
template<typename Alloc> KUN_INLINE bool                               BitArray<Alloc>::empty() { return m_size == 0; }

// operator []
template<typename Alloc> KUN_INLINE BitRef BitArray<Alloc>::operator[](SizeType idx)
{
    KUN_Assert(isValidIndex(idx));
    return BitRef(m_data[idx >> algo::NumBitsPerDWORDLogTwo], 1 << (idx & algo::PerDWORDMask));
}
template<typename Alloc> KUN_INLINE bool BitArray<Alloc>::operator[](SizeType idx) const
{
    KUN_Assert(isValidIndex(idx));
    return m_data[idx >> algo::NumBitsPerDWORDLogTwo] & (1 << (idx & algo::PerDWORDMask));
}

// memory op
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::clear()
{
    algo::setWords(m_data, algo::calcNumWords(m_size), false);
    m_size = 0;
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::release(SizeType capacity)
{
    m_size = 0;
    _resizeTo(capacity);
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::reserve(SizeType capacity)
{
    if (capacity > m_capacity)
        _ResizeTo(capacity);
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::resize(SizeType size, bool new_value) {
    m_size = size;

}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::resizeUnsafe(SizeType size)
{
    m_size = size;
    _resizeGrow();
}

}// namespace kun

// true bit iterator
namespace kun
{
template<typename TS> class TrueBitIt
{
private:
    const u32* m_data;       // data ptr
    TS         m_size;       // data size(in bit)
    TS         m_DWORD_index;// current DWORD index
    TS         m_bit_index;  // current bit index
    u32        m_mask;       // current bit mask
    u32        m_start_mask; // mask to skip start bits
};
template<typename TS> class DualTrueBitIt
{
private:
    const u32* m_data_a;     // data a ptr
    const u32* m_data_b;     // data b ptr
    TS         m_size_a;     // data a size(in bit)
    TS         m_size_b;     // data b size(in bit)
    TS         m_DWORD_index;// current DWORD index
    TS         m_bit_index;  // current bit index
    u32        m_mask;       // current bit mask
    u32        m_start_mask; // mask to skip start bits
};
}// namespace kun