#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"

namespace kun
{
class ArchiveString
{
public:
    // function def
    using GetStrFunc = const char* (*)(void* data);
    using SetStrFunc = void (*)(void* data, const char* str, Size len);
    using StrLenFunc = Size (*)(void* data);

    // help ctor
    KUN_INLINE ArchiveString(void* data, GetStrFunc get_str, SetStrFunc set_str, StrLenFunc str_len)
        : data(data)
        , get_str_func(get_str)
        , set_str_func(set_str)
        , str_len_func(str_len)
    {
    }

    // help func
    KUN_INLINE const char* getStr() const { return get_str_func(data); }
    KUN_INLINE Size strLen() const { return str_len_func(data); }
    KUN_INLINE void setStr(const char* str, Size len) const { return set_str_func(data, str, len); }

public:
    // data
    void* data;
    GetStrFunc get_str_func;
    SetStrFunc set_str_func;
    StrLenFunc str_len_func;
};
}// namespace kun
