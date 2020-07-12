include_directories("${PROJECT_SOURCE_DIR}/tests/include")

# test executables
add_executable(test_with_dataset src/test_with_dataset.cpp)

# # link libraries
target_link_libraries(test_with_dataset PRIVATE bin_packing)

# add test
add_test(test_with_dataset test_with_dataset)
