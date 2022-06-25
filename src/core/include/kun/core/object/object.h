#pragma once
#include "kun/core/std/stl.hpp"

namespace kun
{
using ObjectFlag = u32;
enum ObjectFlag_
{
    ObjectFlag_None = 0,

    // life circle
    ObjectFlag_OnConstruct = 1 << 0,
    ObjectFlag_OnSerialize = 1 << 1,
    ObjectFlag_OnInit = 1 << 2,
    ObjectFlag_OnShutdown = 1 << 3,

    // object state
    ObjectFlag_Complete = 1 << 4,
};

}// namespace kun