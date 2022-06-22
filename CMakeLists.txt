cmake_minimum_required(VERSION 3.16.0)

project(kun VERSION 0.0.0 LANGUAGES CXX)

# config cpp
set(CMAKE_CXX_STANDARD 17)

# config cpp in windows
if(WIN32)
    # shit msvc
    add_compile_definitions(NOMINMAX)

    # prepare for big object
    # add_compile_options(/bigobj)
endif()

# force x64
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message("only support x64 build")
    return()
endif()

# options

# libraries
include(cmake/module.cmake)

# src code
add_subdirectory(src/core)

# test code
add_subdirectory(tests/test_core)