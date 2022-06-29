cmake_minimum_required(VERSION 3.16.0)

# setup vcpkg
if (DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
    message(STATUS "using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}:${VCPKG_TARGET_TRIPLET}")
else ()
    message(SEND_ERROR "lost vcpkg toolchain please set 'VCPKG_ROOT' env value")
endif ()

# fix warning
message(STATUS "use compiler: ${CMAKE_C_COMPILER}")
message(STATUS "c flags: ${CMAKE_C_FLAGS}")

# proj
project(kun VERSION 0.0.0 LANGUAGES CXX)

# config cpp
set(CMAKE_CXX_STANDARD 17)

# config cpp in windows
if (WIN32)
    # shit msvc
    add_compile_definitions(NOMINMAX)

    # enable __cplusplus marco
    add_definitions(/Zc:__cplusplus)

    # enable exception handling
    if (DEFINED CMAKE_C_COMPILER)
        add_definitions(/GX)
    endif ()

    # prepare for big object
    # add_compile_options(/bigobj)
endif ()

# force x64
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message("only support x64 build")
    return()
endif ()

# debug level
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_compile_definitions(KUN_DBG_LEVEL=1)
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
    add_compile_definitions(KUN_DBG_LEVEL=1)
endif ()

# options
option(KUN_WITH_TEST "enable test for kun" ON)

# libraries
include(cmake/global.cmake)
include(cmake/module.cmake)

# src code
add_subdirectory(src/core)

# test code
if (KUN_WITH_TEST)
    enable_testing()
    add_subdirectory(tests/test_core)
endif ()