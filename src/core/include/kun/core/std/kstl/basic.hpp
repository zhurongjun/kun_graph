#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/traits.hpp"

namespace kun
{
template<typename T> KUN_INLINE void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// is swapable
namespace detail
{
template<typename T> using is_swapable = decltype(::kun::swap(std::declval<T&>(), std::declval<T&>()));
}// namespace detail
template<typename T> inline constexpr bool is_swapable_v = is_detected_v<detail::is_swapable, T>;

inline constexpr Size npos = (Size)-1;
}// namespace kun