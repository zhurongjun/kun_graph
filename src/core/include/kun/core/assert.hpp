#pragma once
#include "config.h"
#include "core_api.h"
#include "functional/debugbreak.h"

// debug break
#define KUN_DEBUG_BREAK() debug_break()

// assert marco
#if KUN_DBG_LEVEL >= 1
// abort
#else
    #define KUN_Assertf(expr, fmt, ...)
    #define KUN_Assert(expr)
#endif

// verify marco
#if KUN_DBG_LEVEL >= 1
// debug break & log error
#else
// log error
#endif