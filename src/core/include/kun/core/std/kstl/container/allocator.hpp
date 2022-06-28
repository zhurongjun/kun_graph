#pragma once
#include "kun/core/config.h"
#include "kun/core/functional/assert.hpp"
#include "kun/core/std/types.hpp"
#include "kun/core/memory/memory_resource.h"

// alloc template
namespace kun
{
class __AllocTemplate
{
public:
    using SizeType = Size;

    // impl it
    KUN_INLINE SizeType freeRaw(void*& p, SizeType align);
    KUN_INLINE SizeType reserveRaw(void*& p, SizeType size, SizeType align);

    // helper
    template<typename T> KUN_INLINE SizeType free(T*& p) { return freeRaw(p); }
    template<typename T> KUN_INLINE SizeType reserve(T*& p, SizeType size) { return reserveRaw(p, size * sizeof(T), alignof(size)); }

    // inherent from DefaultChangePolicy
    KUN_INLINE SizeType getGrow(SizeType size, SizeType capacity);  // size > capacity, calc grow
    KUN_INLINE SizeType getShrink(SizeType size, SizeType capacity);// size < capacity, calc shrink
};
}// namespace kun

// change policy
namespace kun
{
template<typename TS> class DefaultChangePolicy
{
    // size > capacity, calc grow
    KUN_INLINE TS getGrow(TS size, TS capacity)
    {
        constexpr TS first_grow = 4;
        constexpr TS constant_grow = 16;

        KUN_Assert(size > capacity && size > 0);

        // init data
        TS result = first_grow;

        // calc grow
        if (capacity || size > first_grow)
        {
            result = size + 3 * size / 8 + constant_grow;
        }

        // handle num over flow
        if (size > result)
            result = std::numeric_limits<TS>::max();

        return result;
    }
    // size < capacity, calc shrink
    KUN_INLINE TS getShrink(TS size, TS capacity)
    {
        TS result;
        KUN_Assert(size < capacity);

        if ((3 * size < 2 * capacity) && (capacity - size > 64 || !size))
        {
            result = size;
        }
        else
        {
            result = capacity;
        }

        return result;
    }
};
}// namespace kun

// pmr allocator
namespace kun
{
class PmrAllocator : public DefaultChangePolicy<Size>
{
    using SizeType = Size;

    // ctor...
    KUN_INLINE PmrAllocator(IMemoryResource* res = defaultMemoryResource())
        : m_res(res)
    {
        KUN_Assert(m_res != nullptr);
    }
    KUN_INLINE PmrAllocator(const PmrAllocator&) = default;
    KUN_INLINE PmrAllocator(PmrAllocator&&) = default;
    KUN_INLINE PmrAllocator& operator=(const PmrAllocator&) = default;
    KUN_INLINE PmrAllocator& operator=(PmrAllocator&&) = default;

    // impl
    KUN_INLINE SizeType freeRaw(void*& p, SizeType align)
    {
        m_res->free(p);
        return 0;
    }
    KUN_INLINE SizeType reserveRaw(void*& p, SizeType size, SizeType align)
    {
        if (p)
        {
            p = m_res->realloc(p, size, align);
        }
        else
        {
            p = m_res->alloc(size, align);
        }
        return size;
    }

    // helper
    template<typename T> KUN_INLINE SizeType free(T*& p) { return freeRaw(p); }
    template<typename T> KUN_INLINE SizeType reserve(T*& p, SizeType size) { return reserveRaw(p, size * sizeof(T), alignof(size)); }

private:
    IMemoryResource* m_res;
};
}// namespace kun