# add library
add_library(point ../include/point.h point.cpp)
add_library(polygon ../include/polygon.h polygon.cpp)
add_library(read ../include/read.h read.cpp)
add_library(sort ../include/sort.h sort.cpp)

target_link_libraries(polygon PRIVATE point)
target_link_libraries(read PRIVATE polygon)
target_link_libraries(sort PRIVATE polygon)

target_include_directories(point PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(polygon PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(read PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(sort PUBLIC "${PROJECT_SOURCE_DIR}/include")