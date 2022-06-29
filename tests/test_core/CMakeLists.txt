# find gtest lib
find_package(GTest REQUIRED)

# add target
kun_add_test(test_core core)

# config libraries
target_link_libraries(test_core PRIVATE core)
