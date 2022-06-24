#pragma once
// !!!! nerver include any header here !!!!

// platform def
#define KUN_PLATFORM_WINDOWS 0
#define KUN_PLATFORM_LINUX 1
#define KUN_PLATFORM_OSX 2
#if defined(_WIN64) || defined(_WIN32)
    #define KUN_PLATFORM KUN_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__linux) || defined(linux)
    #define KUN_PLATFORM KUN_PLATFORM_LINUX
#elif TARGET_OS_OSX
    #define KUN_PLATFORM KUN_PLATFORM_OSX
#else
    #error unsupported platform
#endif

// compiler def
#define KUN_COMPILER_GCC 0
#define KUN_COMPILER_MSVC 1
#define KUN_COMPILER_CLANG 2
#if defined(__GNUC__)
    #define KUN_COMPILER KUN_COMPILER_GCC
#elif defined(_MSC_VER)
    #define KUN_COMPILER KUN_COMPILER_MSVC
#elif defined(__clang__)
    #define KUN_COMPILER KUN_COMPILER_CLANG
#else
    #error unsupported compiler
#endif

// language version
#ifdef __cplusplus
    #if __cplusplus >= 202002L
        #define KUN_CPP_VER 20
    #elif __cplusplus >= 201703L
        #define KUN_CPP_VER 17
    #else
        #error unsupport cpp version, must be 17+
    #endif
#else
    #error unsupport cpp version, must be 17+
#endif

// inline
#if KUN_COMPILER == KUN_COMPILER_GCC || KUN_COMPILER == KUN_COMPILER_CLANG
    #define KUN_INLINE __attribute__((always_inline)) inline
#elif KUN_COMPILER == KUN_COMPILER_MSVC
    #define KUN_INLINE __forceinline
#else
    #define KUN_INLINE inline
#endif

// dllexport
#if KUN_COMPILER == KUN_COMPILER_GCC || KUN_COMPILER == KUN_COMPILER_CLANG
    #define KUN_DLLEXPORT __attribute__((dllexport))
#elif KUN_COMPILER == KUN_COMPILER_MSVC
    #define KUN_DLLEXPORT __declspec(dllexport)
#else
    #error unsupport dll export
#endif

// dllimport
#if KUN_COMPILER == KUN_COMPILER_GCC || KUN_COMPILER == KUN_COMPILER_CLANG
    #define KUN_DLLIMPORT __attribute__((dllimport))
#elif KUN_COMPILER == KUN_COMPILER_MSVC
    #define KUN_DLLIMPORT __declspec(dllimport)
#else
    #error unsupport dll import
#endif

// debug level
// marco   |       KUN_Verify        |  KUN_Assert
// level 0 |       log error         |    none
// level 1 | debug break & log error | debug break
// level 2 | debug break & log error |    abort
#ifndef KUN_DBG_LEVEL
    #define KUN_DBG_LEVEL 0
#endif
