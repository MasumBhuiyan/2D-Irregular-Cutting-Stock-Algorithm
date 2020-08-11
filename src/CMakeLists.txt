# add library
add_library(
    nfp
    "${PROJECT_SOURCE_DIR}/include/libnfporb.hpp"
    libnfporb.cpp
)

add_library(
    bin_packing 
    "${PROJECT_SOURCE_DIR}/include/bin_packing.hpp"
    bin_packing.cpp
)

# link libraries
target_link_libraries(bin_packing nfp)

# include directories  
target_include_directories(
    nfp
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories(
    bin_packing 
    PUBLIC 
    "${PROJECT_SOURCE_DIR}/include"
)