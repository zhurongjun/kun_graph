#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"

namespace kun
{
template<typename T> class Span
{
public:
    KUN_INLINE Span() noexcept
        : m_ptr{nullptr}
        , m_size{0}
    {}
    KUN_INLINE Span(T* ptr, Size len) noexcept
        : m_ptr{ptr}
        , m_size{len}
    {}
    template<Size N>
    KUN_INLINE Span(T (&arr)[N]) noexcept
        : m_ptr(&arr[0])
        , m_size{N}
    {}

    // index access
    KUN_INLINE T& operator[](Size i) const noexcept { return *m_ptr[i]; }

    // size and data
    KUN_INLINE T* data() const noexcept { return m_ptr; }
    KUN_INLINE Size size() const noexcept { return m_size; }

    // front & back
    KUN_INLINE T& front() const noexcept { return *m_ptr; }
    KUN_INLINE T& back() const noexcept { return *(m_ptr + m_size - 1); }

    // sub view
    KUN_INLINE Span<T> first(Size count) { return Span<T>(m_ptr, count); }
    KUN_INLINE Span<T> back(Size count) { return Span<T>(m_ptr + (m_size - count), count); }
    KUN_INLINE Span<T> subSpan(Size offset, Size count) { return Span<T>(m_ptr + offset, count); }

    // foreach support
    KUN_INLINE T* begin() noexcept { return m_ptr; }
    KUN_INLINE T* end() noexcept { return m_ptr + m_size; }

private:
    T* m_ptr;
    Size m_size;
};
}// namespace kun
