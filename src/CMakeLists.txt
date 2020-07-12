# add library
add_library(bin_packing ../include/bin_packing.hpp bin_packing.cpp)

# link libraries


# include directories  
target_include_directories(bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")