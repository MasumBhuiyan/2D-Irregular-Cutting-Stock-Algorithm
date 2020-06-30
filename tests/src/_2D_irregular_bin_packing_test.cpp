#include <_2D_irregular_bin_packing.hpp>

#include <vector>

void _2D_irregular_bin_packing_test1()
{
    Polygon item;
    item.push_back(Point(0, 0));
    item.push_back(Point(0, 8));
    item.push_back(Point(4, 7.5));

    std::vector<Polygon> items({item});
    dnc_approach::solution(items);
}

int main()
{
    _2D_irregular_bin_packing_test1();
}
