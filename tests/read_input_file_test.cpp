#include <string>
#include <vector>
#include "io.hpp"
#include "custom_assertion.hpp"

void read_input_file_test()
{
    string filename = "../tests/io.txt";
    double width = 0;
    vector<polygon> polygons = io::readInputFile(filename, width);
    ASSERT_EQUAL(width, 60.0);
    ASSERT_EQUAL((int)polygons.size(), 2);
    ASSERT_EQUAL(polygons[0].v[1].x, 12.0);
    ASSERT_EQUAL(polygons[1].v[1].x, 12.0);
}

int main()
{
    read_input_file_test();
}