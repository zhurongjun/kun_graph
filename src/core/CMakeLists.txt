# add module
kun_add_module(core)

# check packages
find_package(eastl REQUIRED)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
find_package(mimalloc 2.0 REQUIRED)

# link packages
target_link_libraries(core PUBLIC eastl fmt::fmt-header-only glm mimalloc)
target_link_libraries(core PRIVATE spdlog::spdlog)
