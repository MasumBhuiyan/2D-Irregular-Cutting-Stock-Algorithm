# add library
add_library(bin_packing ../include/bin_packing.hpp bin_packing.cpp)
add_library(libnfporb ../include/libnfporb.hpp libnfporb.cpp)

## link libraries

# include directories  
target_include_directories(bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(libnfporb PUBLIC "${PROJECT_SOURCE_DIR}/include")