#pragma once
#include "kun/core/config.h"
#include "kun/core/std/eastl/eastl_container.hpp"

namespace kun
{
class Exception
{
public:
    virtual ~Exception() = default;
    virtual StringView what() = 0;
};

class StrException : public Exception
{
public:
    KUN_INLINE StrException()
        : m_str("unknown exception")
    {
    }
    KUN_INLINE explicit StrException(StringView str)
        : m_str(str)
    {
    }

    StringView what() override { return m_str; }

private:
    String m_str;
};

}// namespace kun