#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/stl.hpp"
#include "kun/core/std/fmt.hpp"

// log impl
namespace kun::__log
{
KUN_CORE_API void implLogMsg(String log);
KUN_CORE_API void implLogWarn(String log);
KUN_CORE_API void implLogErr(String log);
}// namespace kun::__log

// log template
namespace kun
{
KUN_INLINE void logMsg(StringView str) { __log::implLogMsg(String(str)); }
template<typename... Ts> KUN_INLINE void logMsg(::fmt::format_string<Ts...> fmt, Ts&&... args)
{
    __log::implLogMsg(::kun::format(fmt, std::forward<Ts>(args)...));
}

KUN_INLINE void logWarn(StringView str) { __log::implLogWarn(String(str)); }
template<typename... Ts> KUN_INLINE void logWarn(::fmt::format_string<Ts...> fmt, Ts&&... args)
{
    __log::implLogWarn(::kun::format(fmt, std::forward<Ts>(args)...));
}

KUN_INLINE void logErr(StringView str) { __log::implLogErr(String(str)); }
template<typename... Ts> KUN_INLINE void logErr(::fmt::format_string<Ts...> fmt, Ts&&... args)
{
    __log::implLogErr(::kun::format(fmt, std::forward<Ts>(args)...));
}
}// namespace kun