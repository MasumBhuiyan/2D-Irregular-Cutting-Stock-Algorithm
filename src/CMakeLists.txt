# add library
add_library(_2D_irregular_bin_packing ../include/_2D_irregular_bin_packing.hpp _2D_irregular_bin_packing.cpp)


# target_link_libraries(Point PRIVATE geometry)
# target_link_libraries(Item PRIVATE Point Matrix rasterization)
# target_link_libraries(io PRIVATE Item)
# target_link_libraries(Stock PRIVATE Matrix)
# target_link_libraries(rasterization PRIVATE Matrix geometry Point)
# target_link_libraries(algorithm PRIVATE Matrix)
# target_link_libraries(divide_and_conquer_approach PRIVATE Point Item Matrix geometry)

# target_include_directories(io PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(Point PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(Matrix PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(Item PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(geometry PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(Stock PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(rasterization PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(algorithm PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_include_directories(divide_and_conquer_approach PUBLIC "${PROJECT_SOURCE_DIR}/include")