# add library
add_library(io ../include/io.hpp io.cpp)
add_library(Point ../include/Point.hpp Point.cpp)
add_library(Matrix ../include/Matrix.hpp Matrix.cpp)
add_library(Item ../include/Item.hpp Item.cpp)


target_link_libraries(Item PRIVATE Point)
target_link_libraries(Item PRIVATE Matrix)
target_link_libraries(io PRIVATE Item)

target_include_directories(io PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Point PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Matrix PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Item PUBLIC "${PROJECT_SOURCE_DIR}/include")