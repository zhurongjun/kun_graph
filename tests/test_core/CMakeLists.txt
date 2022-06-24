file(GLOB_RECURSE TEST_CORE_SRC *.cpp)

add_executable(test_core ${TEST_CORE_SRC})

target_link_libraries(test_core PRIVATE core)
