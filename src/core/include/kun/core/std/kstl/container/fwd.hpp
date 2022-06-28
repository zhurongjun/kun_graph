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
}