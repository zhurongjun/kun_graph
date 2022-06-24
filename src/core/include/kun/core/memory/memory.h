#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"

namespace kun
{
// alignment
enum
{
    DEFAULT_ALIGNMENT = 4
};

// malloc
KUN_CORE_API void free(void* p);
KUN_CORE_API void* malloc(size_t n, size_t alignment = DEFAULT_ALIGNMENT);
KUN_CORE_API void* realloc(void* p, size_t n, size_t alignment = DEFAULT_ALIGNMENT);
KUN_CORE_API void* malloc(size_t n, size_t alignment, size_t offset);
KUN_CORE_API void* realloc(void* p, size_t n, size_t alignment, size_t offset);
}// namespace kun