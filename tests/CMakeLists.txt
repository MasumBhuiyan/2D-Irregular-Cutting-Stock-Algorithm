include_directories("${PROJECT_SOURCE_DIR}/tests/include")

# test executables
add_executable(test_with_dataset src/test_with_dataset.cpp)
add_executable(merge_heuristics1_test src/merge_heuristics1_test.cpp)


# # link libraries
target_link_libraries(test_with_dataset PRIVATE _2D_irregular_bin_packing)
target_link_libraries(merge_heuristics1_test PRIVATE _2D_irregular_bin_packing)

# add test
add_test(test_with_dataset test_with_dataset)
add_test(merge_heuristics1_test merge_heuristics1_test)
