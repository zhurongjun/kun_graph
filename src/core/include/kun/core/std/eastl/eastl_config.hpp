#pragma once
#include "kun/core/config.h"
#include "eastl_allocator.h"

// eastl allocator
#ifndef EASTLAllocatorType
    #define EASTLAllocatorType kun::EASTLAllocator
#endif

// eastl allocator get function
#ifndef EASTLAllocatorDefault
    #define EASTLAllocatorDefault kun::eastlDefaultAllocator
#endif

// eastl function SSO size (64 byte[x64] | 32 byte[x86])
#ifndef EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE
    #define EASTL_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE 8 * sizeof(void*)
#endif