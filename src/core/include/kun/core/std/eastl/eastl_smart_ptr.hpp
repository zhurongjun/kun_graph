#pragma once
#include "kun/core/config.h"
#include "kun/core/std/kstl/hash.hpp"
#include "kun/core/memory/memory.h"
#include "kun/core/memory/new_delete.h"

#include "eastl_config.hpp"
#include <EASTL/shared_ptr.h>
#include <EASTL/weak_ptr.h>
#include <EASTL/unique_ptr.h>

namespace kun
{
// deleter for eastl smart ptr
template<typename T> class KunDeleter
{
public:
    constexpr KunDeleter() noexcept = default;

    // Enable if T* can be constructed with U* (i.e. U* is convertible to T*).
    template<typename U> constexpr KunDeleter(const KunDeleter<U>&, typename std::enable_if_t<std::is_convertible_v<U*, T*>>* = 0) noexcept {}

    KUN_INLINE void operator()(T* p) const noexcept { Delete(p); }
};

// smart ptr type
template<typename T> using SPtr = eastl::shared_ptr<T>;
template<typename T> using WPtr = eastl::weak_ptr<T>;
template<typename T> using UPtr = eastl::unique_ptr<T, KunDeleter<T>>;
template<typename T> struct Hash<SPtr<T>>
{
    KUN_INLINE Size operator()(const SPtr<T>& val) const { return eastl::hash<SPtr<T>>()(val); }
};
template<typename T> struct Hash<WPtr<T>>
{
    KUN_INLINE Size operator()(const SPtr<T>& val) const { return eastl::hash<WPtr<T>>()(val); }
};
template<typename T> struct Hash<UPtr<T>>
{
    KUN_INLINE Size operator()(const SPtr<T>& val) const { return eastl::hash<UPtr<T>>()(val); }
};

// make ptr
template<typename T, typename... TArgs> KUN_INLINE SPtr<T> makeSPtr(TArgs... args)
{
    auto ptr = New<T>(std::forward<TArgs>(args)...);
    return SPtr<T>(ptr, KunDeleter<T>());
}
template<typename T, typename... TArgs> KUN_INLINE UPtr<T> makeUPtr(TArgs... args)
{
    auto ptr = New<T>(std::forward<TArgs>(args)...);
    return UPtr<T>(ptr);
}

// raw ptr to ptr
template<typename T> KUN_INLINE SPtr<T> toSPtr(T* p) { return SPtr<T>(p, KunDeleter<T>()); }
template<typename T> KUN_INLINE UPtr<T> toUPtr(T* p) { return UPtr<T>(p); }

// cast
template<typename To, typename From> KUN_INLINE SPtr<To> staticCastSPtr(const SPtr<From>& src) { return SPtr<To>(src, static_cast<To*>(src.get())); }
template<typename To, typename From> KUN_INLINE SPtr<To> constCastSPtr(const SPtr<From>& src) { return SPtr<To>(src, const_cast<To*>(src.get())); }
template<typename To, typename From> KUN_INLINE SPtr<To> reinterpretCastSPtr(const SPtr<From>& src)
{
    return SPtr<To>(src, reinterpret_cast<To*>(src.get()));
}

}// namespace kun