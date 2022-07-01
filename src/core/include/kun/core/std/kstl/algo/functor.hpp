#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/traits.hpp"

// compare functors
namespace kun
{
#define KUN_DEF_COMPARE_FUNCTOR(name, op)                                                                                                            \
    template<typename T = void> struct name                                                                                                          \
    {                                                                                                                                                \
        KUN_INLINE constexpr bool operator()(const T& a, const T& b) const                                                                           \
        {                                                                                                                                            \
            return a op b;                                                                                                                            \
        }                                                                                                                                            \
    };                                                                                                                                               \
    template<> struct name<void>                                                                                                                     \
    {                                                                                                                                                \
        template<typename A, typename B> KUN_INLINE constexpr bool operator()(A&& a, B&& b) const                                                    \
        {                                                                                                                                            \
            return std::forward<A>(a) op std::forward<B>(b);                                                                                          \
        }                                                                                                                                            \
    };

KUN_DEF_COMPARE_FUNCTOR(Less, <)
KUN_DEF_COMPARE_FUNCTOR(Greater, >)
KUN_DEF_COMPARE_FUNCTOR(LessEqual, <=)
KUN_DEF_COMPARE_FUNCTOR(GreaterEqual, >=)
KUN_DEF_COMPARE_FUNCTOR(Equal, ==)
KUN_DEF_COMPARE_FUNCTOR(NotEqual, !=)

#undef KUN_DEF_COMPARE_FUNCTOR
}// namespace kun

// arithmetic functors
namespace kun
{
#define KUN_DEF_ARITHMETIC_FUNCTOR(name, op)                                                                                                         \
    template<typename T = void> struct name                                                                                                          \
    {                                                                                                                                                \
        KUN_INLINE constexpr T operator()(const T& a, const T& b)                                                                                    \
        {                                                                                                                                            \
            return a op b;                                                                                                                           \
        }                                                                                                                                            \
    };                                                                                                                                               \
    template<> struct name<void>                                                                                                                     \
    {                                                                                                                                                \
        template<typename A, typename B> KUN_INLINE constexpr auto operator()(A&& a, B&& b) -> decltype(a op b)                                      \
        {                                                                                                                                            \
            return a op b;                                                                                                                           \
        }                                                                                                                                            \
    };

KUN_DEF_ARITHMETIC_FUNCTOR(OpAdd, +)
KUN_DEF_ARITHMETIC_FUNCTOR(OpSub, -)
KUN_DEF_ARITHMETIC_FUNCTOR(OpMul, *)
KUN_DEF_ARITHMETIC_FUNCTOR(OpDov, /)

#undef KUN_DEF_ARITHMETIC_FUNCTOR
}// namespace kun