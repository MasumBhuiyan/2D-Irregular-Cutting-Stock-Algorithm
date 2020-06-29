include_directories("${PROJECT_SOURCE_DIR}/tests/include")

# test executables
add_executable(_2D_irregular_bin_packing_test src/_2D_irregular_bin_packing_test.cpp)


# # link libraries
target_link_libraries(_2D_irregular_bin_packing_test PRIVATE _2D_irregular_bin_packing)

add_test(_2D_irregular_bin_packing_test _2D_irregular_bin_packing_test)
