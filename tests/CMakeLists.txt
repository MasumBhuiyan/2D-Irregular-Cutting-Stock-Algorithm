include_directories("${PROJECT_SOURCE_DIR}/tests/include")

# test executables
add_executable(generate_nfp_test src/generate_nfp_test.cpp)
add_executable(is_concave_test src/is_concave_test.cpp)

## link libraries
target_link_libraries(generate_nfp_test PRIVATE bin_packing)
target_link_libraries(is_concave_test PRIVATE bin_packing)

# add test
add_test(generate_nfp_test generate_nfp_test)
add_test(is_concave_test is_concave_test)