#pragma once
#include "kun/core/config.h"
#include "kun/core/std/stl.hpp"

namespace kun
{
class ArchiveState
{
public:
    KUN_INLINE ArchiveState()
        : custom_version(0)
        , custom_usage(0)
        , custom_backend(0)
        , is_loading(false)
        , is_saving(false)
        , for_net(false)
        , for_persistent(false)
        , for_cache(false)
        , is_text(false)
        , is_binary(false)
    {
    }

public:
    // user custom data
    u32 custom_version;
    u32 custom_usage;
    u32 custom_backend;

    // serialize usage flag
    u8 is_loading : 1;    // loading usage
    u8 is_saving : 1;     // saving usage
    u8 for_net : 1;       // for pass data by network
    u8 for_persistent : 1;// for persistent save
    u8 for_copy : 1;      // for copy data
    u8 for_cache : 1;     // for cache data

    // backend flag
    u8 is_text : 1;  // text backend, like json/xml
    u8 is_binary : 1;// binary backend
};
}// namespace kun