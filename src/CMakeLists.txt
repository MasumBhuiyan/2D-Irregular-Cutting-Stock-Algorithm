# add library
add_library(_2D_irregular_bin_packing ../include/_2D_irregular_bin_packing.hpp _2D_irregular_bin_packing.cpp)

target_include_directories(_2D_irregular_bin_packing PUBLIC "${PROJECT_SOURCE_DIR}/include")