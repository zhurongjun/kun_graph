function(kun_add_module module_name)
    # process name
    string(TOUPPER ${module_name} upper_module_name)

    # collect files
    file(GLOB_RECURSE SRC_FILES src/*.cpp)

    # add target
    add_library(${module_name} SHARED ${SRC_FILES})

    # add include
    target_include_directories(${module_name} PUBLIC include/)

    # add export def
    target_compile_definitions(${module_name} PRIVATE "EXPORT_${upper_module_name}")

    # set output dir
    set_target_properties(${module_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${KUN_MODULE_DIR}/${module_name})
    set_target_properties(${module_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KUN_LIBRARY_DIR}/${module_name})
    set_target_properties(${module_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${KUN_LIBRARY_DIR}/${module_name})
endfunction()

function(kun_add_test test_name)
    # collect files
    file(GLOB_RECURSE SRC_FILES *.cpp)

    # add target
    add_executable(${test_name} ${SRC_FILES})

    # add packages
    target_link_libraries(${test_name} PRIVATE GTest::gtest_main)

    # set output dir
    set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${KUN_RUNTIME_DIR})
    set_target_properties(${test_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KUN_RUNTIME_DIR})
    set_target_properties(${test_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${KUN_RUNTIME_DIR})
endfunction()

