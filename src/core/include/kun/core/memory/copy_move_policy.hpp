#pragma once
#include "kun/core/config.h"
#include "memory.h"

// traits
namespace kun
{
// can copy construct behaviour use memcpy
template<typename T, typename U> struct is_bitwise_copyable
{
    static_assert(!std::is_lvalue_reference_v<T> && !std::is_lvalue_reference_v<U>, "is_bitwise_copyable is not designed to accept reference types");

    static_assert(std::is_same_v<T, std::remove_cv_t<T>> && std::is_same_v<U, std::remove_cv_t<U>>,
                  "is_bitwise_copyable is not designed to accept qualified types");

    static constexpr bool value = false;
};
template<typename T> struct is_bitwise_copyable<T, T>
{
    static constexpr bool value = std::is_trivially_copy_constructible_v<T>;
};
template<typename T, typename U> struct is_bitwise_copyable<const T, U> : public is_bitwise_copyable<T, U>
{
};
template<typename T> struct is_bitwise_copyable<const T*, T*>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<u8, i8>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<i8, u8>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<u16, i16>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<i16, u16>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<u32, i32>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<i32, u32>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<u64, i64>
{
    static constexpr bool value = true;
};
template<> struct is_bitwise_copyable<i64, u64>
{
    static constexpr bool value = true;
};
template<typename T, typename U> inline constexpr bool is_bitwise_copyable_v = is_bitwise_copyable<T, U>::value;

// can assign behaviour use memcpy
template<typename T, typename U> struct is_bitwise_assignable
{
    static constexpr bool value = std::is_trivially_assignable_v<std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<U>>;
};
template<typename T, typename U> inline constexpr bool is_bitwise_assignable_v = is_bitwise_assignable<T, U>::value;

// can compare behaviour use memcmp
template<typename T, typename U> struct is_bitwise_comparable
{
    static constexpr bool value = false;
};
template<typename T> struct is_bitwise_comparable<T, T>
{
    static constexpr bool value = std::is_trivial_v<T>;
};
template<typename T, typename U> inline constexpr bool is_bitwise_comparable_v = is_bitwise_comparable<T, U>::value;

}// namespace kun

namespace kun
{
// construct & destruct
template<typename T> KUN_INLINE void constructItems(T* p, Size count)
{
    if constexpr (!std::is_trivially_constructible_v<T>)
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
    if constexpr (!std::is_trivially_destructible_v<T>)
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
    if constexpr (is_bitwise_copyable_v<Dst, Src>)
    {
        memory::memcpy(dst, src, sizeof(Src) * count);
    }
    else
    {
        while (count)
        {
            new (dst) Dst(*src);
            ++dst;
            ++src;
            --count;
        }
    }
}
template<typename Dst, typename Src> KUN_INLINE void assignItems(Dst* dst, Src* src, Size count)
{
    if constexpr (is_bitwise_assignable_v<Dst, Src>)
    {
        memory::memcpy(dst, src, sizeof(Src) * count);
    }
    else
    {
        while (count)
        {
            *dst = *src;
            ++dst;
            ++src;
            --count;
        }
    }
}

// move copy & move assign
template<typename Dst, typename Src> KUN_INLINE void moveCopyItems(Dst* dst, Src* src, Size count)
{
    if constexpr (is_bitwise_copyable_v<Dst, Src>)
    {
        memory::memmove(dst, src, sizeof(Src) * count);
    }
    else
    {
        while (count)
        {
            new (dst) Dst(std::move(*src));
            ++dst;
            ++src;
            --count;
        }
    }
}
template<typename Dst, typename Src> KUN_INLINE void moveAssignItems(Dst* dst, Src* src, Size count)
{
    if constexpr (is_bitwise_assignable_v<Dst, Src>)
    {
        memory::memmove(dst, src, sizeof(Src) * count);
    }
    else
    {
        while (count)
        {
            *dst = std::move(*src);
            ++dst;
            ++src;
            --count;
        }
    }
}

// compare
template<typename A, typename B> KUN_INLINE bool compareItems(const A* a, const B* b, Size count)
{
    if constexpr (is_bitwise_comparable_v<A, B>)
    {
        return !memcmp(a, b, sizeof(B) * count);
    }
    else
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
}
}// namespace kun