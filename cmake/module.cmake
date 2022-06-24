function(kun_add_module module_name)
    # process name
    string(TOUPPER ${module_name} upper_module_name)

    # collect file
    file(GLOB_RECURSE SRC_FILES src/*.cpp)

    # add target
    add_library(${module_name} SHARED ${SRC_FILES})

    # add include
    target_include_directories(${module_name} PUBLIC include/)

    # add export def
    target_compile_definitions(${module_name} PRIVATE "EXPORT_${upper_module_name}")

    # set output dir
    set_target_properties(${module_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${KUN_LIBRARY_OUTPUT_DIR})
    set_target_properties(${module_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${KUN_RUNTIME_OUTPUT_DIR})
    set_target_properties(${module_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${KUN_ARCHIVE_OUTPUT_DIR})
endfunction()