# add library
add_library(io ../include/io.hpp io.cpp)
add_library(geometry ../include/geometry.hpp geometry.cpp)
add_library(heuristics ../include/heuristics.hpp heuristics.cpp)
add_library(evaluation ../include/evaluation.hpp evaluation.cpp)

target_link_libraries(io PRIVATE geometry)

target_include_directories(io PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(geometry PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(heuristics PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(evaluation PUBLIC "${PROJECT_SOURCE_DIR}/include")