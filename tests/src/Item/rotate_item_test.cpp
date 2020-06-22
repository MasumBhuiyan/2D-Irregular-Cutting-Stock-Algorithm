#include <iostream>

#include "Item.hpp"
#include "geometry.hpp"
#include "custom_assertion.hpp"

void rotate_item_test()
{
    Item item(3, vector<Point>({Point(100, 100),
                                Point(100, 200),
                                Point(200, 200)}));

    Item rotatedItem = item.rotate(geo::DEG2RAD(-45), Point(50, -50));
    rotatedItem.print();
    /*ASSERT_EQUAL(geo::dcmp(rotatedItem.vertices[0].x - 191.421, 1e-3), 0);
    ASSERT_EQUAL(geo::dcmp(rotatedItem.vertices[0].y - 20.7107, 1e-3), 0);
    ASSERT_EQUAL(geo::dcmp(rotatedItem.vertices[1].x - 262.132, 1e-3), 0);
    ASSERT_EQUAL(geo::dcmp(rotatedItem.vertices[1].y - 91.4214, 1e-3), 0);
    ASSERT_EQUAL(geo::dcmp(rotatedItem.vertices[2].x - 332.843, 1e-3), 0);
    ASSERT_EQUAL(geo::dcmp(rotatedItem.vertices[2].y - 20.7107, 1e-3), 0);*/
}

int main()
{
    rotate_item_test();
}