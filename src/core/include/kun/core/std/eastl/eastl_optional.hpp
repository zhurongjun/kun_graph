#pragma once
#include "kun/core/config.h"
#include "kun/core/std/kstl/hash.hpp"

#include "eastl_config.hpp"
#include <EASTL/optional.h>

namespace kun
{
// static inline constexpr eastl::nullopt_t nullopt = eastl::nullopt;
// static inline constexpr eastl::in_place_t in_place = eastl::in_place;
// template<typename T> using Optional = eastl::optional<T>;
//
// template<typename T, typename... TArgs> KUN_INLINE Optional<T> makeOptional(TArgs&&... args)
// {
//     return eastl::make_optional<T>(std::forward<TArgs>(args)...);
// }
}// namespace kun