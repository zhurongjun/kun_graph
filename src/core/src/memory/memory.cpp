#include "kun/core/memory/memory.h"
#include <mimalloc.h>

namespace kun::memory
{
// malloc
void free(void* p) { mi_free(p); }
void* malloc(Size n, Size alignment) { return mi_malloc_aligned(n, alignment); }
void* realloc(void* p, Size n, Size alignment) { return mi_realloc_aligned(p, n, alignment); }
void* malloc(Size n, Size alignment, Size offset) { return mi_malloc_aligned_at(n, alignment, offset); }
void* realloc(void* p, Size n, Size alignment, Size offset) { return mi_realloc_aligned_at(p, n, alignment, offset); }

// memops
void* memcpy(void* dst, const void* src, Size size) { return ::memcpy(dst, src, size); }

}// namespace kun::memory