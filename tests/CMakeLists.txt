add_executable(demo_test demo_test.cpp)
add_executable(read_input_file_test read_input_file_test.cpp)

# link libraries
target_link_libraries(read_input_file_test PRIVATE io)


add_test(demo_test demo_test)
add_test(read_input_file_test read_input_file_test)

