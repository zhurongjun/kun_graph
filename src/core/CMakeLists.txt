# collect cpp files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# add target
add_library(core SHARED ${SRC_FILES})

# add include
target_include_directories(core PUBLIC include/)

# add export def
target_compile_definitions(core PRIVATE "EXPORT_CORE")

# set output dir
set_target_properties(core PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${KUN_RUNTIME_DIR})
set_target_properties(core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KUN_LIBRARY_DIR})
set_target_properties(core PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${KUN_LIBRARY_DIR})

# check packages
find_package(eastl REQUIRED)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(mimalloc 2.0 REQUIRED)

# setup glm
target_compile_definitions(core PUBLIC GLM_SWIZZLE)

# link packages
target_link_libraries(core PUBLIC EASTL fmt::fmt-header-only glm::glm)
target_link_libraries(core PRIVATE mimalloc-static spdlog::spdlog)
