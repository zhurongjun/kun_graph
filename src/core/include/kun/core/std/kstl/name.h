#pragma once
#include "kun/core/config.h"
#include "kun/core/core_api.h"
#include "kun/core/std/eastl/eastl_container.hpp"

namespace kun
{
static inline constexpr Size MAX_NAME_LEN = 128;

class KUN_CORE_API Name
{
public:
    // ctor
    KUN_INLINE Name(StringView str = {});

    // copy construct & assign
    KUN_INLINE Name(const Name&) = default;
    KUN_INLINE Name(Name&&) = default;
    KUN_INLINE Name& operator=(const Name&) = default;
    KUN_INLINE Name& operator=(Name&&) = default;

    // getter
    Size length() const;
    StringView str() const;

    // compare
    friend bool operator==(const Name& lhs, const Name& rhs);
    friend bool operator!=(const Name& lhs, const Name& rhs);
    friend bool operator>=(const Name& lhs, const Name& rhs);
    friend bool operator<=(const Name& lhs, const Name& rhs);
    friend bool operator>(const Name& lhs, const Name& rhs);
    friend bool operator<(const Name& lhs, const Name& rhs);

private:
    Size m_idx;
};
}// namespace kun

namespace kun
{
KUN_INLINE bool operator==(const Name& lhs, const Name& rhs) { return lhs.m_idx == rhs.m_idx; }
KUN_INLINE bool operator!=(const Name& lhs, const Name& rhs) { return lhs.m_idx != rhs.m_idx; }
KUN_INLINE bool operator>=(const Name& lhs, const Name& rhs) { return lhs.m_idx >= rhs.m_idx; }
KUN_INLINE bool operator<=(const Name& lhs, const Name& rhs) { return lhs.m_idx <= rhs.m_idx; }
KUN_INLINE bool operator>(const Name& lhs, const Name& rhs) { return lhs.m_idx > rhs.m_idx; }
KUN_INLINE bool operator<(const Name& lhs, const Name& rhs) { return lhs.m_idx < rhs.m_idx; }
}// namespace kun