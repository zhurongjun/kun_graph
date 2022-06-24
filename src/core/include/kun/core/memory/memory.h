#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/types.hpp"

namespace kun::memory
{
// alignment
enum
{
    DEFAULT_ALIGNMENT = 4
};

// malloc
KUN_CORE_API void free(void* p);
KUN_CORE_API void* malloc(Size n, Size alignment = DEFAULT_ALIGNMENT);
KUN_CORE_API void* realloc(void* p, Size n, Size alignment = DEFAULT_ALIGNMENT);
KUN_CORE_API void* malloc(Size n, Size alignment, Size offset);
KUN_CORE_API void* realloc(void* p, Size n, Size alignment, Size offset);

// memory ops
KUN_CORE_API void* memcpy(void* dst, const void* src, Size size);
}// namespace kun::memory