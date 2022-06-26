#pragma once
#include "kun/core/config.h"
#include "kun/core/std/traits.hpp"
#include "kun/core/std/types.hpp"

#define KUN_IMPL_HASH_CAST(type)                                                                                                                     \
    template<> struct Hash<type>                                                                                                                     \
    {                                                                                                                                                \
        KUN_INLINE Size operator()(type val) const                                                                                                   \
        {                                                                                                                                            \
            return static_cast<Size>(val);                                                                                                           \
        }                                                                                                                                            \
    };

#define KUN_IMPL_HASH_ADAPTER(type, adapter)                                                                                                         \
    template<> struct Hash<type>                                                                                                                     \
    {                                                                                                                                                \
        KUN_INLINE Size operator()(const type& val) const                                                                                            \
        {                                                                                                                                            \
            return adapter<type>()(val);                                                                                                             \
        }                                                                                                                                            \
    };

namespace kun
{
// hash structure
template<typename T> struct Hash
{
};

// impl for pointer
template<typename T> struct Hash<T*>
{
    KUN_INLINE Size operator()(T* val) const { return Size((void*)val); }
};

// impl for int
KUN_IMPL_HASH_CAST(i8)
KUN_IMPL_HASH_CAST(u8)
KUN_IMPL_HASH_CAST(i16)
KUN_IMPL_HASH_CAST(u16)
KUN_IMPL_HASH_CAST(i32)
KUN_IMPL_HASH_CAST(u32)
KUN_IMPL_HASH_CAST(i64)
KUN_IMPL_HASH_CAST(u64)

// impl for float
KUN_IMPL_HASH_CAST(f32)
KUN_IMPL_HASH_CAST(f64)

// hash combine
KUN_INLINE Size hash_combine(Size seed, Size combine) { return seed ^ (combine + 0x9e3779b9 + (seed << 6) + (seed >> 2)); }
template<typename T> KUN_INLINE void hash_combine(Size& seed, const T& v) { seed = hash_combine(seed, Hash<T>()(v)); }
}// namespace kun

#undef KUN_IMPL_HASH_CAST
