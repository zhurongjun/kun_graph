#pragma once
#include "kun/core/config.h"
#include "kun/core/std/traits.hpp"
#include "kun/core/std/types.hpp"

// hash def
namespace kun
{
// hash structure
template<typename T> struct Hash
{
};
}// namespace kun

// impl for basic types
namespace kun
{
#define KUN_IMPL_HASH_CAST(type)                                                                                                                     \
    template<> struct Hash<type>                                                                                                                     \
    {                                                                                                                                                \
        KUN_INLINE Size operator()(type val) const                                                                                                   \
        {                                                                                                                                            \
            return static_cast<Size>(val);                                                                                                           \
        }                                                                                                                                            \
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

#undef KUN_IMPL_HASH_CAST
}// namespace kun

// impl for string
// [from eastl] https://github.com/electronicarts/EASTL/blob/master/include/EASTL/functional.h
namespace kun
{
// char 8
template <> struct Hash<char*>
{
    Size operator()(const char* p) const
    {
        uint32_t c, result = 2166136261U;   // FNV1 hash. Perhaps the best string hash. Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
        while((c = (uint8_t)*p++) != 0)     // Using '!=' disables compiler warnings.
            result = (result * 16777619) ^ c;
        return (Size)result;
    }
};
template <> struct Hash<const char*>
{
    Size operator()(const char* p) const
    {
        uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
        while((c = (uint8_t)*p++) != 0)     // cast to unsigned 8 bit.
            result = (result * 16777619) ^ c;
        return (Size)result;
    }
};

// char 16
template <> struct Hash<char16_t*>
{
    Size operator()(const char16_t* p) const
    {
        uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
        while((c = (uint16_t)*p++) != 0)    // cast to unsigned 16 bit.
            result = (result * 16777619) ^ c;
        return (Size)result;
    }
};

template <> struct Hash<const char16_t*>
{
    Size operator()(const char16_t* p) const
    {
        uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
        while((c = (uint16_t)*p++) != 0)    // cast to unsigned 16 bit.
            result = (result * 16777619) ^ c;
        return (Size)result;
    }
};

// char 32
template <> struct Hash<char32_t*>
{
    Size operator()(const char32_t* p) const
    {
        uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
        while((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
            result = (result * 16777619) ^ c;
        return (Size)result;
    }
};

template <> struct Hash<const char32_t*>
{
    Size operator()(const char32_t* p) const
    {
        uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
        while((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
            result = (result * 16777619) ^ c;
        return (Size)result;
    }
};
}

// hash combine
namespace kun
{
KUN_INLINE Size                      hash_combine(Size seed, Size combine) { return seed ^ (combine + 0x9e3779b9 + (seed << 6) + (seed >> 2)); }
template<typename T> KUN_INLINE void hash_combine(Size& seed, const T& v) { seed = hash_combine(seed, Hash<T>()(v)); }
}// namespace kun

// traits
namespace kun
{
namespace detail
{
template<typename T> using is_hashable = decltype(std::declval<Hash<T>>().operator()(std::declval<T&>()));
}// namespace detail
template<typename T> inline constexpr bool is_hashable_v = is_detected_v<detail::is_hashable, T>;
}// namespace kun

// adapter from other hasher e.g. eastl::hash ==> KUN_IMPL_HASH_ADAPTER(String, eastl::hash)
#define KUN_IMPL_HASH_ADAPTER(type, adapter)                                                                                                         \
    template<> struct Hash<type>                                                                                                                     \
    {                                                                                                                                                \
        KUN_INLINE Size operator()(const type& val) const                                                                                            \
        {                                                                                                                                            \
            return adapter<type>()(val);                                                                                                             \
        }                                                                                                                                            \
    };
