cmake_minimum_required(VERSION 3.10)

# project name
project(run)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# executable file
add_executable(main main.cpp)

# headers implementations
add_subdirectory(src)



target_include_directories(
    main
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Enable testing globally. Individual tests may be found in the `tests`
# subdirectory of the repository.
# enable_testing()
# add_subdirectory(tests)
