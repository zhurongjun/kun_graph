#pragma once
#include "kun/core/config.h"
#include "kun/core/std/traits.hpp"
#include "kun/core/std/types.hpp"
#include "memory.h"

namespace kun
{
// new flag
using NewFlag = u32;
enum NewFlag_
{
    NewFlag_None = 0,
};

// new & delete
template<typename T, NewFlag flag = NewFlag_None, typename... TArgs> KUN_INLINE T* New(TArgs&&... args)
{
    void* p = memory::malloc(sizeof(T), alignof(T));
    T* p_obj = new (p) T(std::forward<TArgs>(args)...);
    return p_obj;
}
template<typename T> inline void Delete(T* o)
{
    o->~T();
    memory::free(o);
}

}// namespace kun
