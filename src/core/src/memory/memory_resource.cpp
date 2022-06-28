#include "kun/core/memory/memory_resource.h"

namespace kun::memory
{
class DefaultMemoryResource : public IMemoryResource
{
    void* alloc(Size size, Size alignment) override { return ::kun::memory::malloc(size, alignment); }
    void* realloc(void* p, Size size, Size alignment) override { return ::kun::memory::realloc(p, size, alignment); }
    void free(void* p) override { ::kun::memory::free(p); }
};

IMemoryResource* defaultMemoryResource()
{
    static DefaultMemoryResource instance;
    return &instance;
}
}// namespace kun::memory