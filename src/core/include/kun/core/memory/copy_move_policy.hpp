#pragma once
#include "kun/core/config.h"
#include "memory.h"

// traits
namespace kun::memory
{
// memory policy traits
template<typename A, typename B = A> struct memory_policy_traits
{
    static constexpr bool call_ctor = true;
    static constexpr bool call_dtor = true;
    static constexpr bool call_copy = true;
    static constexpr bool call_move = true;
    static constexpr bool call_assign = true;
    static constexpr bool call_move_assign = true;
    static constexpr bool call_compare = true;
    static constexpr bool use_realloc = false;
};
template<typename T> struct memory_policy_traits<T, T>
{
    static constexpr bool call_ctor = !std::is_trivially_constructible_v<T>;
    static constexpr bool call_dtor = !std::is_trivially_destructible_v<T>;
    static constexpr bool call_copy = !std::is_trivially_copyable_v<T>;
    static constexpr bool call_move = !std::is_trivially_move_constructible_v<T>;
    static constexpr bool call_assign = !std::is_trivially_assignable_v<std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<T>>;
    static constexpr bool call_move_assign = !std::is_trivially_move_assignable_v<T>;
    static constexpr bool call_compare = !std::is_trivial_v<T>;
    static constexpr bool use_realloc = std::is_trivial_v<T>;
};
template<typename T> struct memory_policy_traits<T*, T*>
{
    static constexpr bool call_ctor = false;
    static constexpr bool call_dtor = false;
    static constexpr bool call_copy = false;
    static constexpr bool call_move = false;
    static constexpr bool call_assign = false;
    static constexpr bool call_move_assign = false;
    static constexpr bool call_compare = false;
    static constexpr bool use_realloc = true;
};
template<typename A, typename B> struct memory_policy_traits<const A, B> : public memory_policy_traits<A, B>
{
};

#define KUN_IMPL_BASIC_MEM_POLICY(dst, src)                                                                                                          \
    template<> struct memory_policy_traits<dst, src>                                                                                                 \
    {                                                                                                                                                \
        static constexpr bool call_ctor = false;                                                                                                     \
        static constexpr bool call_dtor = false;                                                                                                     \
        static constexpr bool call_copy = false;                                                                                                     \
        static constexpr bool call_move = false;                                                                                                     \
        static constexpr bool call_assign = false;                                                                                                   \
        static constexpr bool call_move_assign = false;                                                                                              \
        static constexpr bool call_compare = true;                                                                                                   \
        static constexpr bool use_realloc = true;                                                                                                    \
    };

KUN_IMPL_BASIC_MEM_POLICY(u8, i8)
KUN_IMPL_BASIC_MEM_POLICY(i8, u8)
KUN_IMPL_BASIC_MEM_POLICY(u16, i16)
KUN_IMPL_BASIC_MEM_POLICY(i16, u16)
KUN_IMPL_BASIC_MEM_POLICY(u32, i32)
KUN_IMPL_BASIC_MEM_POLICY(i32, u32)
KUN_IMPL_BASIC_MEM_POLICY(u64, i64)
KUN_IMPL_BASIC_MEM_POLICY(i64, u64)

#undef KUN_IMPL_BASIC_MEM_POLICY
}// namespace kun::memory

namespace kun::memory
{
// construct & destruct
template<typename T> KUN_INLINE void constructItems(T* p, Size count)
{
    if constexpr (memory_policy_traits<T>::call_ctor)
    {
        while (count)
        {
            new (p) T();
            ++p;
            --count;
        }
    }
}
template<typename T> KUN_INLINE void destructItem(T* p, Size count)
{
    if constexpr (memory_policy_traits<T>::call_dtor)
    {
        while (count)
        {
            using __DestructItemTTypeDef = T;

            p->__DestructItemTTypeDef::~__DestructItemTTypeDef();
            ++p;
            --count;
        }
    }
}

// copy & assign
template<typename Dst, typename Src> KUN_INLINE void copyItems(Dst* dst, Src* src, Size count)
{
    if constexpr (memory_policy_traits<Dst, Src>::call_copy)
    {
        while (count)
        {
            new (dst) Dst(*src);
            ++dst;
            ++src;
            --count;
        }
    }
    else
    {
        memory::memcpy(dst, src, sizeof(Src) * count);
    }
}
template<typename Dst, typename Src> KUN_INLINE void assignItems(Dst* dst, Src* src, Size count)
{
    if constexpr (memory_policy_traits<Dst, Src>::call_assign)
    {
        while (count)
        {
            *dst = *src;
            ++dst;
            ++src;
            --count;
        }
    }
    else
    {
        memory::memcpy(dst, src, sizeof(Src) * count);
    }
}

// move copy & move assign
template<typename Dst, typename Src> KUN_INLINE void moveCopyItems(Dst* dst, Src* src, Size count)
{
    if constexpr (memory_policy_traits<Dst, Src>::call_move)
    {
        while (count)
        {
            new (dst) Dst(std::move(*src));
            ++dst;
            ++src;
            --count;
        }
    }
    else
    {
        memory::memmove(dst, src, sizeof(Src) * count);
    }
}
template<typename Dst, typename Src> KUN_INLINE void moveAssignItems(Dst* dst, Src* src, Size count)
{
    if constexpr (memory_policy_traits<Dst, Src>::call_move_assign)
    {
        while (count)
        {
            *dst = std::move(*src);
            ++dst;
            ++src;
            --count;
        }
    }
    else
    {
        memory::memmove(dst, src, sizeof(Src) * count);
    }
}

// compare
template<typename A, typename B> KUN_INLINE bool compareItems(const A* a, const B* b, Size count)
{
    if constexpr (memory_policy_traits<A, B>::call_compare)
    {
        while (count)
        {
            if (!(*a == *a))
            {
                return false;
            }

            ++a;
            ++b;
            --count;
        }
        return true;
    }
    else
    {
        return !memcmp(a, b, sizeof(B) * count);
    }
}
}// namespace kun::memory