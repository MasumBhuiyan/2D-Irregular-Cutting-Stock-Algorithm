# add library
add_library(bin_packing ../include/bin_packing.hpp bin_packing.cpp)
add_library(geo_util ../include/geo_util.hpp geo_util.cpp)
add_library(polygon_fit ../include/polygon_fit.hpp polygon_fit.cpp)
add_library(cluster_util ../include/cluster_util.hpp cluster_util.cpp)

## link libraries
target_link_libraries(geo_util PRIVATE bin_packing)
target_link_libraries(polygon_fit PRIVATE geo_util)
target_link_libraries(cluster_util PRIVATE geo_util polygon_fit bin_packing)

# include directories  
target_include_directories(bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(geo_util PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(polygon_fit PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(cluster_util PUBLIC "${PROJECT_SOURCE_DIR}/include")