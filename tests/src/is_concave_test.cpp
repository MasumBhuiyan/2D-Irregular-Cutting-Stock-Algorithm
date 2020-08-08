#include <bin_packing.hpp>
#include <cassert>

void is_concave_test()
{
    Polygon polygon1, polygon2, polygon3;
    boost_geo::read_wkt("POLYGON((1e-08 -5,5 1e-08,5 5,1e-08 5,-5 5,-5 1e-08,-5 -5,1e-08 -5))", polygon1);
    boost_geo::read_wkt("POLYGON((10 -5,10 1e-08,5 5,1e-08 5,1e-08 1e-08,5 -5,10 -5))", polygon2);
    boost_geo::read_wkt("POLYGON((0 0,0 10, 5 5, 10 10,10 0,0 0))", polygon3);
    geo_util::visualize({polygon1}, "../../tests/results/", "isConcave_test1");
    geo_util::visualize({polygon2}, "../../tests/results/", "isConcave_test2");
    geo_util::visualize({polygon3}, "../../tests/results/", "isConcave_test3");

    bool a = geo_util::isConcave(polygon1);
    bool b = geo_util::isConcave(polygon2);
    bool c = geo_util::isConcave(polygon3);

    std::cout << a << " " << b << " " << c << "\n";
   // assert(geo_util::isConcave(polygon1) == false);
   // assert(geo_util::isConcave(polygon2) == false);
   // assert(geo_util::isConcave(polygon3) == true);
}

int main()
{
    is_concave_test();
    return 0;
}