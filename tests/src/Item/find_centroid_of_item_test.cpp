#include "Item.hpp"
#include "geometry.hpp"
#include "custom_assertion.hpp"

void find_centroid_of_item_test1()
{
    Item item(3, {{1, 0}, {2, 0}, {0, 3}});
    ASSERT_EQUAL(geo::dcmp(item.pivotPoint.x - 1), 0);
    ASSERT_EQUAL(geo::dcmp(item.pivotPoint.y - 1), 0);
}

void find_centroid_of_item_test2()
{
    Item item(5, {{1, 0}, {2, 1}, {0, 3}, {-1, 2}, {-2, -1}});
    ASSERT_EQUAL(geo::dcmp(item.pivotPoint.x - (-1.0 / 12.0)), 0);
    ASSERT_EQUAL(geo::dcmp(item.pivotPoint.y - (11.0 / 12.0)), 0);
}

int main()
{
    find_centroid_of_item_test1();
    find_centroid_of_item_test2();
}