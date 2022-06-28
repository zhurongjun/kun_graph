#pragma once
#include "kun/core/config.h"
#include "kun/core/std/types.hpp"
#include "kun/core/std/kstl/container/bit_array.hpp"

// bit ref
namespace kun
{
class BitRef
{
public:
    KUN_INLINE BitRef(u32& data, u32 mask)
        : m_data(data)
        , m_mask(mask)
    {
    }
    KUN_INLINE operator bool() const { return (m_data & m_mask); }
    KUN_INLINE void operator=(bool v) { m_data = v ? m_data | m_mask : m_data & ~m_mask; }
    KUN_INLINE BitRef& operator=(const BitRef& rhs)
    {
        *this = (bool)rhs;
        return *this;
    }

private:
    u32& m_data;
    u32 m_mask;
};
}// namespace kun

// bit iterator
namespace kun
{
template<typename TS, bool Const> class BitIt
{
    using DataPtr = std::conditional_t<Const, const u32*, u32*>;
    using Ref = std::conditional_t<Const, bool, BitRef>;
    
private:
    DataPtr* m_data;
    u32 m_mask;
    TS m_size;
    TS m_index;
    TS m_DWORDIndex;
};

}// namespace kun
