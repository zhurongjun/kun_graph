#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/algo/bit_array.hpp"
#include "kun/core/memory/memory.h"
#include "bit_iterator.hpp"
#include "fwd.hpp"

// bit array def
namespace kun
{
template<typename Alloc> class BitArray final
{
public:
    using SizeType = typename Alloc::SizeType;
    using It = BitIt<SizeType, false>;
    using CIt = BitIt<SizeType, true>;

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

    // contain
    bool contain(bool v) const;

    // set range
    void setRange(SizeType start, SizeType n, bool v);

    // validate
    bool isValidIndex(SizeType idx);

    // support foreach
    BitIt<SizeType, false> begin();
    BitIt<SizeType, false> end();
    BitIt<SizeType, true>  begin() const;
    BitIt<SizeType, true>  end() const;

private:
    // helper
    void _grow(SizeType size);
    void _resize(SizeType size);

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
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::_grow(SizeType size)
{
    if (m_size + size > m_capacity)
    {
        // calc new capacity
        SizeType old_word_size = algo::calcNumWords(m_size);
        SizeType old_word_capacity = algo::calcNumWords(m_capacity);
        SizeType new_word_size = algo::calcNumWords(m_size + size);
        SizeType new_word_capacity = m_allocator.getGrow(new_word_size, old_word_capacity);

        // realloc
        m_data = m_allocator.resizeContainer(m_data, old_word_size, old_word_capacity, new_word_capacity);

        // update capacity
        m_capacity = new_word_capacity << algo::NumBitsPerDWORDLogTwo;
    }
    // update size
    m_size += size;
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::_resize(SizeType size)
{
    SizeType old_word_capacity = algo::calcNumWords(m_capacity);
    SizeType new_word_capacity = algo::calcNumWords(size);

    if (new_word_capacity)
    {
        // realloc
        m_data = m_allocator.resizeContainer(m_data, m_size, old_word_capacity, new_word_capacity);

        // clean new memory
        if (new_word_capacity > old_word_capacity)
            memory::memzero(data() + old_word_capacity, (new_word_capacity - old_word_capacity) * sizeof(u32));

        // change capacity
        m_capacity = new_word_capacity * algo::NumBitsPerDWORD;
    }
    else
    {
        // free
        m_allocator.free(m_data);
        m_capacity = 0;
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
    _resize(other.m_capacity);

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

    _resize(rhs.m_size);
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
    _resize(capacity);
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::reserve(SizeType capacity)
{
    if (capacity > m_capacity)
        _resize(capacity);
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::resize(SizeType size, bool new_value)
{
    // set size
    auto old_size = m_size;
    m_size = size;

    // do resize
    if (size > m_capacity)
    {
        _resize(size);
    }

    // try init
    if (size > m_size)
    {
        setRange(old_size, size, new_value);
    }
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::resizeUnsafe(SizeType size)
{
    // do resize
    if (size > m_capacity)
    {
        _resize(size);
    }

    // set size
    m_size = size;
}

// push
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::SizeType BitArray<Alloc>::add(bool v)
{
    _grow(1);

    // increment size
    auto old_size = m_size;
    ++m_size;

    // set value
    (*this)[old_size] = v;
    return old_size;
}
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::SizeType BitArray<Alloc>::add(bool v, SizeType n)
{
    _grow(n);

    // increment size
    auto old_size = m_size;
    m_size += n;

    // set value
    setRange(old_size, m_size, v);
    return old_size;
}

// remove
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::removeAt(SizeType start, SizeType n)
{
    KUN_Assert(start >= 0 && n > 0 && start + n < m_size);
    if (start + n != m_size)
    {
        It write(*this, start);
        It read(*this, start + n);

        while (read)
        {
            *write = *read;
            ++read;
        }
    }
    m_size -= n;
}
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::removeAtSwap(SizeType start, SizeType n)
{
    KUN_Assert(start >= 0 && n > 0 && start + n < m_size);
    if (start + n != m_size)
    {
        // adjust n
        n = std::min(n, m_size - start - n);

        // copy items
        for (SizeType i = 0; i < n; ++i) { (*this)[start + i] = (*this)[m_size - n + i]; }
    }
    m_size -= n;
}

// find
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::SizeType BitArray<Alloc>::find(bool v) const
{
    return algo::findBit(m_data, m_size, v);
}
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::SizeType BitArray<Alloc>::findLast(bool v) const
{
    return algo::findLastBit(m_data, m_size, v);
}

// contain
template<typename Alloc> KUN_INLINE bool BitArray<Alloc>::contain(bool v) const { return find(v) != npos; }

// set range
template<typename Alloc> KUN_INLINE void BitArray<Alloc>::setRange(SizeType start, SizeType n, bool v)
{
    KUN_Assert(start >= 0 && n > 0 && start + n <= m_size);
    algo::setBitRange(m_data, start, n, v);
}

// validate
template<typename Alloc> KUN_INLINE bool BitArray<Alloc>::isValidIndex(SizeType idx) { return idx >= 0 && idx < m_size; }

// support foreach
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::It  BitArray<Alloc>::begin() { return It(*this); }
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::It  BitArray<Alloc>::end() { return It(*this, m_size); }
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::CIt BitArray<Alloc>::begin() const { return CIt(*this); }
template<typename Alloc> KUN_INLINE typename BitArray<Alloc>::CIt BitArray<Alloc>::end() const { return CIt(*this, m_size); }

}// namespace kun
