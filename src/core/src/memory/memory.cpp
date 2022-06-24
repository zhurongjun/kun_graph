#include "kun/core/memory/memory.h"
#include <mimalloc.h>

namespace kun
{
void free(void* p) { mi_free(p); }
void* malloc(size_t n, size_t alignment) { return mi_malloc_aligned(n, alignment); }
void* realloc(void* p, size_t n, size_t alignment) { return mi_realloc_aligned(p, n, alignment); }
void* malloc(size_t n, size_t alignment, size_t offset) { return mi_malloc_aligned_at(n, alignment, offset); }
void* realloc(void* p, size_t n, size_t alignment, size_t offset) { return mi_realloc_aligned_at(p, n, alignment, offset); }
}// namespace kun