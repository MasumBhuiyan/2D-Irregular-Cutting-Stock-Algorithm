# add library
add_library(boost_geometry_utility ../include/boost_geometry_utility.hpp boost_geometry_utility.cpp)
add_library(_2D_irregular_bin_packing ../include/_2D_irregular_bin_packing.hpp _2D_irregular_bin_packing.cpp)

# link libraries
target_link_libraries(_2D_irregular_bin_packing boost_geometry_utility)

# include directories
target_include_directories(boost_geometry_utility PUBLIC "${PROJECT_SOURCE_DIR}/include")    
target_include_directories(_2D_irregular_bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")