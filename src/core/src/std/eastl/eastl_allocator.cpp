#include "kun/core/std/eastl/eastl_allocator.h"

namespace kun
{
EASTLAllocator* eastlDefaultAllocator()
{
    static EASTLAllocator ins;
    return &ins;
}
}// namespace kun