#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"

namespace kun
{
template<typename T> struct NamedValue
{
public:
    StringView name;
    T& value;

    KUN_INLINE NamedValue(StringView name, T& value)
        : name(name)
        , value(value)
    {
    }
    KUN_INLINE NamedValue(T& value)
        : name()
        , value(value)
    {
    }
};
}// namespace kun

#define KUN_NAMED(val) NamedValue<std::decay_t<decltype(val)>>(#val, val)