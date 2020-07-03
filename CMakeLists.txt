# # Created by the script cgal_create_CMakeLists
# # This is the CMake script for compiling a set of CGAL applications.

# project( 2D-Irregular-Cutting-Stock-Algorithm )


# cmake_minimum_required(VERSION 3.10)

# # CGAL and its components
# find_package( CGAL QUIET COMPONENTS  )

# if ( NOT CGAL_FOUND )

#   message(STATUS "This project requires the CGAL library, and will not be compiled.")
#   return()  

# endif()

# # include helper file
# include( ${CGAL_USE_FILE} )


# # Boost and its components
# find_package( Boost REQUIRED )

# if ( NOT Boost_FOUND )

#   message(STATUS "This project requires the Boost library, and will not be compiled.")

#   return()  

# endif()

# # include for local directory
# include_directories( BEFORE include )

# # include for local package


# # Creating entries for target: run
# # ############################

# add_executable( run  main.cpp )

# add_to_cached_list( CGAL_EXECUTABLE_TARGETS run )


# # headers implementations
# add_subdirectory(src)

# # Link the executable to CGAL and third-party libraries
# target_link_libraries(
#   run   
#   _2D_irregular_bin_packing
#   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} 
# )

# target_include_directories(
#     run
#     PUBLIC
#     "${PROJECT_SOURCE_DIR}/include"
# )

# # Enable testing globally. Individual tests may be found in the `tests`
# # subdirectory of the repository.
# enable_testing()
# add_subdirectory(tests)

cmake_minimum_required(VERSION 3.10)

# project name
project(2D-Irregular-Cutting-Stock-Algorithm)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# executable file
add_executable(run main.cpp)

# headers implementations
add_subdirectory(src)

target_link_libraries(
    run
    _2D_irregular_bin_packing
)

target_include_directories(
    run
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Enable testing globally. Individual tests may be found in the `tests`
# subdirectory of the repository.
enable_testing()
add_subdirectory(tests)