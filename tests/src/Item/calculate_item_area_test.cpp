#include "Item.hpp"
#include "custom_assertion.hpp"

void calculate_item_area_test()
{
    Item item(6, {{2.66, 4.71},
                  {5, 3.5},
                  {3.63, 2.52},
                  {4, 1.5},
                  {1.9, 1},
                  {0.72, 2.28}});
    ASSERT_EQUAL(geo::dcmp(item.area - 8.4458), 0);
}

int main()
{
    calculate_item_area_test();
}