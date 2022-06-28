#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/types.hpp"
#include "memory.h"

// allocator interface
namespace kun
{
class IMemoryResource
{
public:
    virtual ~IMemoryResource() = default;

    // alloc
    virtual void* alloc(Size size, Size alignment) = 0;
    virtual void* realloc(void* p, Size size, Size alignment) = 0;
    virtual void free(void* p) = 0;

    // help
    template<typename T> KUN_INLINE T* alloc(Size count = 1) { return (T*)alloc(count * sizeof(T), alignof(T)); }
    template<typename T> KUN_INLINE T* realloc(T* p, Size count = 1) { return (T*)realloc(p, count * sizeof(T), alignof(T)); }
    template<typename T> KUN_INLINE void free(T* p) { free(p); }
};
}// namespace kun

// default allocator
namespace kun
{
KUN_CORE_API IMemoryResource* defaultMemoryResource();
}