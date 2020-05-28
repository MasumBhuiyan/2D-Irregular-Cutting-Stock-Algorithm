cmake_minimum_required(VERSION 3.10)

# project name
project(2D_Irregular_Cutting_Stock_Problem)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# executable file
add_executable(2D_Irregular_Cutting_Stock_Problem main.cpp)

# headers implementations
add_subdirectory(src)

target_link_libraries(
    2D_Irregular_Cutting_Stock_Problem
    PRIVATE
    io 
    geometry 
    heuristics
    evaluation 
)

target_include_directories(
    2D_Irregular_Cutting_Stock_Problem
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)