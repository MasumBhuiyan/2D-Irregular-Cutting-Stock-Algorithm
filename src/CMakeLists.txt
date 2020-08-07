# add library
add_library(bin_packing ../include/bin_packing.hpp bin_packing.cpp)
add_library(libnfporb ../include/libnfporb.hpp libnfporb.cpp)

## link libraries
target_link_libraries(bin_packing PRIVATE libnfporb)

# include directories  
target_include_directories(bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(libnfporb PUBLIC "${PROJECT_SOURCE_DIR}/include")