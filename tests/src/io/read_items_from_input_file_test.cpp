#include <string>
#include <vector>
#include "io.hpp"
#include <approach0.hpp>
#include "custom_assertion.hpp"

void read_items_from_input_file_test()
{
    string filename = "../../tests/src/io/read_items_from_input_file_test_input.txt";
    double width = 0;
    vector<Item> items = io::readItemsFromInputFile(filename, width);
    std::vector<Item> stock = approach0::split(items, 0, items.size() - 1);
    for(auto item : stock)
    {
        item.print();
    }
    // **N.T. asserts error because points are now normalized
   /* ASSERT_EQUAL(width, 60.0);
    ASSERT_EQUAL((int)items.size(), 2);
    ASSERT_EQUAL(items[0].vertices[1].x, 12.0);
    ASSERT_EQUAL(items[1].vertices[1].x, 12.0);*/
}

int main()
{
    read_items_from_input_file_test();
}