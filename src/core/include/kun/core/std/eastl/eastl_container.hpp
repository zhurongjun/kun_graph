#pragma once
#include "kun/core/config.h"
#include "kun/core/std/kstl/hash.hpp"

#include "eastl_config.hpp"
#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unordered_set.h>
#include <EASTL/string.h>
#include <EASTL/tuple.h>
#include <EASTL/variant.h>

namespace kun
{
// containers
// template<typename T> using Array = eastl::vector<T>;
template<typename T> using USet = eastl::unordered_set<T, Hash<T>>;
template<typename K, typename V> using UMap = eastl::unordered_map<K, V, Hash<K>>;

// string
using String = eastl::string;
using StringView = eastl::string_view;
KUN_IMPL_HASH_ADAPTER(String, eastl::hash)
KUN_IMPL_HASH_ADAPTER(StringView, eastl::hash)

// function
template<typename T> using Func = eastl::function<T>;

// tuple
template<typename... Args> using Tuple = eastl::tuple<Args...>;
template<typename... Args> struct Hash<Tuple<Args...>>
{
    KUN_INLINE Size operator()(const Tuple<Args...>& val) const { return eastl::hash<Tuple<Args...>>()(val); }
};

// pair
template<typename K, typename V> using Pair = eastl::pair<K, V>;
template<typename K, typename V> struct Hash<Pair<K, V>>
{
    KUN_INLINE Size operator()(const Pair<K, V>& val) const { return eastl::hash<Pair<K, V>>()(val); }
};

// variant
template<typename... Args> using Variant = eastl::variant<Args...>;

}// namespace kun