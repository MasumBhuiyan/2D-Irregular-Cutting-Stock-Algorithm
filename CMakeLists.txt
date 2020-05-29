cmake_minimum_required(VERSION 3.10)

# project name
project(run)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# executable file
add_executable(run main.cpp)

# headers implementations
add_subdirectory(src)

target_link_libraries(
    run
    PRIVATE
    io 
    geometry 
    heuristics
    evaluation 
)

target_include_directories(
    run
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)
