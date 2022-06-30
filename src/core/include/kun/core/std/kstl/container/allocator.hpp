#pragma once
#include "kun/core/config.h"
#include "kun/core/functional/assert.hpp"
#include "kun/core/std/types.hpp"
#include "kun/core/memory/memory_resource.h"

// alloc template
namespace kun
{
template<typename TDerived, typename TS> class AllocTemplate
{
public:
    using SizeType = TS;

    // impl it
    // KUN_INLINE void  freeRaw(void* p, SizeType align);
    // KUN_INLINE void* allocRaw(SizeType size, SizeType align);
    // KUN_INLINE void* reallocRaw(void* p, SizeType size, SizeType align);

    // helper
    template<typename T> KUN_INLINE void free(T* p) { static_cast<TDerived*>(this)->freeRaw(p, alignof(T)); }
    template<typename T> KUN_INLINE T*   alloc(SizeType size) { return (T*)static_cast<TDerived*>(this)->allocRaw(size * sizeof(T), alignof(T)); }
    template<typename T> KUN_INLINE T*   realloc(T* p, SizeType size)
    {
        return (T*)static_cast<TDerived*>(this)->reallocRaw(p, size * sizeof(T), alignof(T));
    }

    // size > capacity, calc grow
    KUN_INLINE SizeType getGrow(SizeType size, SizeType capacity)
    {
        constexpr SizeType first_grow = 4;
        constexpr SizeType constant_grow = 16;

        KUN_Assert(size > capacity && size > 0);

        // init data
        SizeType result = first_grow;

        // calc grow
        if (capacity || size > first_grow)
        {
            result = size + 3 * size / 8 + constant_grow;
        }

        // handle num over flow
        if (size > result)
            result = std::numeric_limits<SizeType>::max();

        return result;
    }
    // size < capacity, calc shrink
    KUN_INLINE SizeType getShrink(SizeType size, SizeType capacity)
    {
        SizeType result;
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

    // resize functional
    template<typename T> KUN_INLINE T* resizeContainer(T* p, SizeType size, SizeType capacity, SizeType new_capacity)
    {
        KUN_Assert(new_capacity > 0);
        KUN_Assert(size <= capacity);
        KUN_Assert((capacity > 0 && p != nullptr) || (capacity == 0 && p == nullptr));

        if constexpr (memory::memory_policy_traits<T>::use_realloc)
        {
            if (size && (size > capacity / 3 * 2 || size >= new_capacity))
            {
                return realloc(p, new_capacity);
            }
        }

        // alloc new memory
        T* new_memory = alloc<T>(new_capacity);

        // move memory
        if (size)
        {
            // move items
            memory::moveItems(new_memory, p, std::min(size, new_capacity));

            // destruct old items
            memory::destructItem(p, size);

            // release old memory
            free(p);
        }

        return new_memory;
    }
};
}// namespace kun

// pmr allocator
namespace kun
{
class PmrAllocator : public AllocTemplate<PmrAllocator, Size>
{
public:
    // ctor...
    KUN_INLINE PmrAllocator(IMemoryResource* res = defaultMemoryResource())
        : m_res(res)
    {
        KUN_Assert(m_res != nullptr);
    }
    KUN_INLINE               PmrAllocator(const PmrAllocator&) = default;
    KUN_INLINE               PmrAllocator(PmrAllocator&&) = default;
    KUN_INLINE PmrAllocator& operator=(const PmrAllocator&) = default;
    KUN_INLINE PmrAllocator& operator=(PmrAllocator&&) = default;

    // impl
    KUN_INLINE void  freeRaw(void* p, SizeType align) { m_res->free(p); }
    KUN_INLINE void* allocRaw(SizeType size, SizeType align) { return m_res->alloc(size, align); }
    KUN_INLINE void* reallocRaw(void* p, SizeType size, SizeType align) { return m_res->realloc(p, size, align); }

private:
    IMemoryResource* m_res;
};
}// namespace kun