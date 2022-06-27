#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"

// force header only
#ifndef FMT_HEADER_ONLY
    #define FMT_HEADER_ONLY 1
#endif

// fmt include
#pragma warning(push)
#pragma warning(disable : 4582)
#pragma warning(disable : 4583)
#pragma warning(disable : 4459)
#include "fmt/core.h"
#pragma warning(pop)

// string impl
namespace fmt
{
template<> struct formatter<kun::String, kun::String::value_type> : formatter<basic_string_view<kun::String::value_type>, kun::String::value_type>
{
    template<typename FormatContext> FMT_CONSTEXPR auto format(const kun::String& val, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return formatter<basic_string_view<kun::String::value_type>, kun::String::value_type>::format(
            basic_string_view<kun::String::value_type>(val.data(), val.length()), ctx);
    }
};
template<>
struct formatter<kun::StringView, kun::StringView::value_type>
    : formatter<basic_string_view<kun::StringView::value_type>, kun::StringView::value_type>
{
    template<typename FormatContext> FMT_CONSTEXPR auto format(kun::StringView val, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return formatter<basic_string_view<kun::StringView::value_type>, kun::StringView::value_type>::format(
            basic_string_view<kun::StringView::value_type>(val.data(), val.length()), ctx);
    }
};
}// namespace fmt

// global format function
namespace kun
{
template<typename... Ts> KUN_INLINE String format(::fmt::format_string<Ts...> fmt, Ts&&... args)
{
    String str;
    ::fmt::vformat_to(std::back_inserter(str), fmt, ::fmt::make_format_args(std::forward<Ts>(args)...));
    return str;
}

template<typename... Ts> KUN_INLINE String format_s(StringView fmt, Ts&&... args)
{
    String str;
    ::fmt::vformat_to(std::back_inserter(str), std::string_view(fmt.data(), fmt.length()), ::fmt::make_format_args(std::forward<Ts>(args)...));
    return str;
}
}// namespace kun