#pragma once
#include "kun/core/config.h"

#if EXPORT_CORE
    #define KUN_CORE_API KUN_DLLEXPORT
#else
    #define KUN_CORE_API KUN_DLLIMPORT
#endif