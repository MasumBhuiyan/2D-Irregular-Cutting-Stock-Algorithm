# add library
add_library(bin_packing ../include/bin_packing.hpp bin_packing.cpp)
add_library(geo_util ../include/geo_util.hpp geo_util.cpp)

## link libraries
target_link_libraries(geo_util PRIVATE bin_packing)

# include directories  
target_include_directories(bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(geo_util PUBLIC "${PROJECT_SOURCE_DIR}/include")