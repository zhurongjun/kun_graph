#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "log.h"
#include "debugbreak.h"
#include <cstdlib>
#include <iostream>

// debug break
#define KUN_DEBUG_BREAK() debug_break()

// dump error template
namespace kun
{
KUN_INLINE void dumpAssertError(String err, const char* file, Size line)
{
    String full_msg = ::kun::format("assert failed!!!!\nError:\t\t{}\nSource:\t\t{}(line: {})", err, file, line);
    ::kun::logErr(full_msg);
    ::std::cerr << full_msg.data() << std::endl;
}

KUN_INLINE void dumpVerifyError(String err, const char* file, Size line)
{
    ::kun::logErr("verify failed!!!!\nError:\t\t{}\nSource:\t\t{}(line: {})", err, file, line);
}
}// namespace kun

// assert marco
#if KUN_DBG_LEVEL >= 1
    #define KUN_Assert(expr)                                                                                                                         \
        {                                                                                                                                            \
            if (!(expr))                                                                                                                             \
            {                                                                                                                                        \
                dumpAssertError(::kun::format("\"{}\" failed!!!", #expr), __FILE__, __LINE__);                                                       \
                std::abort();                                                                                                                        \
            }                                                                                                                                        \
        }
    #define KUN_Assertf(expr, fmt, ...)                                                                                                              \
        {                                                                                                                                            \
            if (!(expr))                                                                                                                             \
            {                                                                                                                                        \
                dumpAssertError(::kun::format("\"{}\" failed!!!\nMessage:\t" fmt, #expr, __VA_ARGS__), __FILE__, __LINE__);                          \
                std::abort();                                                                                                                        \
            }                                                                                                                                        \
        }
#else
    #define KUN_Assertf(expr, fmt, ...)
    #define KUN_Assert(expr)
#endif

// verify marco
#if KUN_DBG_LEVEL >= 1
    #define KUN_Verify(expr)                                                                                                                         \
        {                                                                                                                                            \
            if (!(expr))                                                                                                                             \
            {                                                                                                                                        \
                dumpVerifyError(::kun::format("\"{}\" failed!!!", #expr), __FILE__, __LINE__);                                                       \
                KUN_DEBUG_BREAK();                                                                                                                   \
            }                                                                                                                                        \
        }
    #define KUN_Verifyf(expr, fmt, ...)                                                                                                              \
        {                                                                                                                                            \
            if (!(expr))                                                                                                                             \
            {                                                                                                                                        \
                dumpVerifyError(::kun::format("\"{}\" failed!!!\nMessage:\t" fmt, #expr, __VA_ARGS__), __FILE__, __LINE__);                          \
                KUN_DEBUG_BREAK();                                                                                                                   \
            }                                                                                                                                        \
        }
#else
    #define KUN_Verify(expr)                                                                                                                         \
        {                                                                                                                                            \
            if (!(expr))                                                                                                                             \
            {                                                                                                                                        \
                dumpVerifyError(::kun::format("\"{}\" failed!!!", #expr), __FILE__, __LINE__);                                                       \
            }                                                                                                                                        \
        }
    #define KUN_Verifyf(expr, fmt, ...)                                                                                                              \
        {                                                                                                                                            \
            if (!(expr))                                                                                                                             \
            {                                                                                                                                        \
                dumpVerifyError(::kun::format("\"{}\" failed!!!\nMessage:\t" fmt, #expr, __VA_ARGS__), __FILE__, __LINE__);                          \
            }                                                                                                                                        \
        }
#endif