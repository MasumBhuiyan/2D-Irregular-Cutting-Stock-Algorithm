include_directories("${PROJECT_SOURCE_DIR}/tests/include")

# test executables
add_executable(geo_util_test src/geo_util_test.cpp)

## link libraries
target_link_libraries(geo_util_test geo_util)

# add test
add_test(geo_util_test geo_util_test)
