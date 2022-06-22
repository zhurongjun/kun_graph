function(kun_add_module module_name)
    # collect file
    file(GLOB_RECURSE SRC_FILES src/*.cpp)

    # add target
    add_library(${module_name} SHARED ${SRC_FILES})

    # include dir
    target_include_directories(${module_name} PUBLIC ./include)
endfunction()