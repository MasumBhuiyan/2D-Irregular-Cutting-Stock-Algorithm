include_directories("${PROJECT_SOURCE_DIR}/tests/include")

# test executables
add_executable(geo_util_test src/geo_util_test.cpp)
add_executable(cluster_util_test src/cluster_util_test.cpp)

## link libraries
target_link_libraries(geo_util_test geo_util)
target_link_libraries(cluster_util_test cluster_util)

# add test
add_test(geo_util_test geo_util_test)
add_test(cluster_util_test cluster_util_test)