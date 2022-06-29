#pragma once

// custom std for cross-platform/cross-stdlib interface, include
//  - array                 [eastl]
//  - uset                  [eastl]
//  - umap                  [eastl]
//  - string/string_view    [eastl]
//  - function              [eastl]
//  - tuple/pair            [eastl]
//  - smart ptr             [eastl]
//  - optional              [eastl]
//  - variant               [eastl]
//---------------------------------------
//  - name(unique string)   [kstl]
//  - exception             [kstl]
//  - hash                  [kstl]
//  - span                  [kstl]
//  - any                   [kstl]
//  - bit array             [kstl]

// from eastl
#include "eastl/eastl_allocator.h"
#include "eastl/eastl_config.hpp"
#include "eastl/eastl_container.hpp"
#include "eastl/eastl_smart_ptr.hpp"
#include "eastl/eastl_optional.hpp"

// from kstl
#include "kstl/basic.hpp"
#include "kstl/span.hpp"
#include "kstl/hash.hpp"
#include "kstl/exception.hpp"
#include "kstl/name.h"
#include "kstl/container/fwd.hpp"
#include "kstl/container/allocator.hpp"
#include "kstl/container/bit_array.hpp"
#include "kstl/container/array.hpp"
#include "kstl/container/sparse_array.hpp"
#include "kstl/container/uset.hpp"
#include "kstl/container/umap.hpp"