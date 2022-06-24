#pragma once
#include <eastl/algorithm.h>

#if EXPORT_CORE
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif