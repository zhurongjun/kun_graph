file(GLOB_RECURSE TEST_CORE_SRC *.cpp)

add_executable(test_core ${TEST_CORE_SRC})

target_link_libraries(test_core PRIVATE core)

set_target_properties(test_core PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${KUN_LIBRARY_OUTPUT_DIR})
set_target_properties(test_core PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${KUN_RUNTIME_OUTPUT_DIR})
set_target_properties(test_core PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KUN_ARCHIVE_OUTPUT_DIR})