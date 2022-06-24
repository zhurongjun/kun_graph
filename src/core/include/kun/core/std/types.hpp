#pragma once
#include <cstdint>
#include <limits>
#include "kun/core/config.h"

#define KUN_INT_CONSTANTS(size)                                                                                                                      \
    inline constexpr i##size I##size##_Min = std::numeric_limits<i##size>::min();                                                                    \
    inline constexpr i##size I##size##_MAX = std::numeric_limits<i##size>::max();                                                                    \
    inline constexpr u##size U##size##_Min = std::numeric_limits<u##size>::min();                                                                    \
    inline constexpr u##size U##size##_MAX = std::numeric_limits<u##size>::max();

#define KUN_FLT_CONSTANTS(size)                                                                                                                      \
    inline constexpr f##size F##size##_MIN = std::numeric_limits<f##size>::min();                                                                    \
    inline constexpr f##size F##size##_MAX = std::numeric_limits<f##size>::max();                                                                    \
    inline constexpr f##size F##size##_NAN = std::numeric_limits<f##size>::quiet_NaN();                                                              \
    inline constexpr f##size F##size##_SNAN = std::numeric_limits<f##size>::signaling_NaN();                                                         \
    inline constexpr f##size F##size##_EPSILON = std::numeric_limits<f##size>::epsilon();                                                            \
    inline constexpr f##size F##size##_INFINITY = std::numeric_limits<f##size>::infinity();                                                          \
    inline constexpr f##size F##size##_TOLERANCE = ((f##size)0.00001);

namespace kun
{
// integer types
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;

// float type
using f32 = float;
using f64 = double;

// size type
using Size = std::size_t;

// integer limits
KUN_INT_CONSTANTS(8)
KUN_INT_CONSTANTS(16)
KUN_INT_CONSTANTS(32)
KUN_INT_CONSTANTS(64)

// float constants
KUN_FLT_CONSTANTS(32)
KUN_FLT_CONSTANTS(64)

}// namespace kun

#undef KUN_INT_CONSTANTS
#undef KUN_FLT_CONSTANTS