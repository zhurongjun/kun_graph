#pragma once

// allocator
namespace kun
{
class PmrAllocator;
using DefaultAllocator = PmrAllocator;
}

// containers
namespace kun
{
template<typename Alloc = DefaultAllocator> class BitArray;
template<typename T, typename Alloc = DefaultAllocator> class Array;
template<typename T, typename Alloc = DefaultAllocator> class SparseArray;

template<typename T, bool MultiKey = false> struct USetConfigDefault;
template<typename T, typename Alloc = DefaultAllocator, typename Config = USetConfigDefault<T>> class USet;

}// namespace kun