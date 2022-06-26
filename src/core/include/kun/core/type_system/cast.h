#pragma once
#include "kun/core/config.h"

namespace kun
{
template<typename To, typename From> To KUN_INLINE cast(From from) { return dynamic_cast<To>(from); }
}// namespace kun