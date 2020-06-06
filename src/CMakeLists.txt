# add library
add_library(io ../include/io.hpp io.cpp)
add_library(Point ../include/Point.hpp Point.cpp)
add_library(Matrix ../include/Matrix.hpp Matrix.cpp)
add_library(Item ../include/Item.hpp Item.cpp)
add_library(geometry ../include/geometry.hpp geometry.cpp)
add_library(Stock ../include/Stock.hpp Stock.cpp)


target_link_libraries(Point PRIVATE geometry)
target_link_libraries(Item PRIVATE Point Matrix)
target_link_libraries(io PRIVATE Item)
target_link_libraries(Stock PRIVATE Matrix)

target_include_directories(io PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Point PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Matrix PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Item PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(geometry PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(Stock PUBLIC "${PROJECT_SOURCE_DIR}/include")