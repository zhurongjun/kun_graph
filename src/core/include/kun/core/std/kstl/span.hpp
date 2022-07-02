#pragma once
#include "kun/core/config.h"
#include "algo/find.hpp"
#include "kun/core/std/types.hpp"

// Span def
namespace kun
{
template<typename T> class Span
{
public:
    // ctor & dtor
    Span();
    Span(T* p, Size len);
    template<Size N> Span(T (&arr)[N]);
    ~Span();

    // copy & move & assign & move assign
    Span(const Span& other);
    Span(Span&& other);
    Span& operator=(const Span& other);
    Span& operator=(Span&& other);

    // compare
    bool operator==(const Span& rhs) const;
    bool operator!=(const Span& rhs) const;

    // getter
    T*   data() const;
    Size size() const;

    // validate
    bool isValidIndex(Size idx) const;
    bool isValidPointer(const T* p) const;

    // sub span
    Span<T> left(Size pos) const;
    Span<T> right(Size pos) const;
    Span<T> subSpan(Size pos, Size n) const;

    // find
    template<typename TK> T* find(const TK& v) const;
    template<typename TK> T* findLast(const TK& v) const;

    // find if
    template<typename TP> T* findIf(TP&& p) const;
    template<typename TP> T* findLastIf(TP&& p) const;

    // contain
    bool contain(const T& v) const;

    // contain if
    template<typename TP> bool containIf(TP&& p) const;

    // top & bottom
    T& top() const;
    T& bottom() const;

    // foreach support
    T* begin() const;
    T* end() const;

private:
    T*   m_data;
    Size m_size;
};
}// namespace kun

// Spam impl
namespace kun
{
// ctor & dtor
template<typename T>
KUN_INLINE Span<T>::Span()
    : m_data(nullptr)
    , m_size(0)
{
}
template<typename T>
KUN_INLINE Span<T>::Span(T* p, Size len)
    : m_data(p)
    , m_size(len)
{
}
template<typename T>
template<Size N>
KUN_INLINE Span<T>::Span(T (&arr)[N])
    : m_data(arr)
    , m_size(N)
{
}
template<typename T> KUN_INLINE Span<T>::~Span() = default;

// copy & move & assign & move assign
template<typename T> KUN_INLINE Span<T>::Span(const Span& other) = default;
template<typename T> KUN_INLINE Span<T>::Span(Span&& other) = default;
template<typename T> KUN_INLINE Span<T>& Span<T>::operator=(const Span& other) = default;
template<typename T> KUN_INLINE Span<T>& Span<T>::operator=(Span&& other) = default;

// compare
template<typename T> KUN_INLINE bool Span<T>::operator==(const Span& rhs) const { return m_data == rhs.m_data && m_size == rhs.size(); }
template<typename T> KUN_INLINE bool Span<T>::operator!=(const Span& rhs) const { return !(*this == rhs); }

// getter
template<typename T> KUN_INLINE T*   Span<T>::data() const { return m_data; }
template<typename T> KUN_INLINE Size Span<T>::size() const { return m_size; }

// validate
template<typename T> KUN_INLINE bool Span<T>::isValidIndex(Size idx) const { return idx >= 0 && idx < m_size; }
template<typename T> KUN_INLINE bool Span<T>::isValidPointer(const T* p) const { return p >= begin() && p < end(); }

// sub span
template<typename T> KUN_INLINE Span<T> Span<T>::left(Size pos) const
{
    KUN_Assert(pos < m_size);
    return Span(m_data, pos);
}
template<typename T> KUN_INLINE Span<T> Span<T>::right(Size pos) const
{
    KUN_Assert(pos < m_size);
    return pos < m_size - 1 ? Span(m_data + pos, m_size - pos) : Span();
}
template<typename T> KUN_INLINE Span<T> Span<T>::subSpan(Size pos, Size n) const
{
    KUN_Assert(pos < m_size && pos + n < m_size);
    return Span(m_size + pos, n);
}

// find
template<typename T> template<typename TK> KUN_INLINE T* Span<T>::find(const TK& v) const
{
    return findIf([&v](const T& a) { return a == v; });
}
template<typename T> template<typename TK> KUN_INLINE T* Span<T>::findLast(const TK& v) const
{
    return findLastIf([&v](const T& a) { return a == v; });
}

// find if
template<typename T> template<typename TP> KUN_INLINE T* Span<T>::findIf(TP&& p) const { return algo::find(begin(), end(), std::forward<TP>(p)); }
template<typename T> template<typename TP> KUN_INLINE T* Span<T>::findLastIf(TP&& p) const
{
    return algo::findLast(begin(), end(), std::forward<TP>(p));
}

// contain
template<typename T> KUN_INLINE bool Span<T>::contain(const T& v) const { return find(v); }

// contain if
template<typename T> template<typename TP> KUN_INLINE bool Span<T>::containIf(TP&& p) const { return findIf(std::forward<TP>(p)); }

// top & bottom
template<typename T> KUN_INLINE T& Span<T>::top() const { return *(m_data + m_size - 1); }
template<typename T> KUN_INLINE T& Span<T>::bottom() const { return *m_data; }

// foreach support
template<typename T> KUN_INLINE T* Span<T>::begin() const { return m_data; }
template<typename T> KUN_INLINE T* Span<T>::end() const { return m_data + m_size; }

}// namespace kun