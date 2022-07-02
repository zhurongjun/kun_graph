#pragma once

// allocator
namespace kun
{
class PmrAllocator;
}

// containers
namespace kun
{
template<typename Alloc = PmrAllocator> class BitArray;
template<typename T, typename Alloc = PmrAllocator> class Array;
template<typename T, typename Alloc = PmrAllocator> class SparseArray;
}// namespace kun