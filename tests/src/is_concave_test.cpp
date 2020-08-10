#include <bin_packing.hpp>
#include <cassert>

void is_concave_test()
{
    // Polygon polygon1, polygon2, polygon3, polygon4, polygon5, polygon6;
    // boost_geo::read_wkt("POLYGON((1e-08 -5,5 1e-08,5 5,1e-08 5,-5 5,-5 1e-08,-5 -5,1e-08 -5))", polygon1);
    // boost_geo::read_wkt("POLYGON((10 -5,10 1e-08,5 5,1e-08 5,1e-08 1e-08,5 -5,10 -5))", polygon2);
    // boost_geo::read_wkt("POLYGON((0 0,0 10, 5 5, 10 10,10 0,0 0))", polygon3);
    // boost_geo::read_wkt("POLYGON((1e-08 -5,5 1e-08,5 5,1e-08 5,-5 5,-5 1e-08,-5 -5,1e-08 -5))", polygon4);
    // boost_geo::read_wkt("POLYGON((5 1e-08,5 5,5 10,1e-08 10,-5 10,1e-08 5,5 1e-08))", polygon5);
    
    // // geo_util::visualize({polygon1}, "../../tests/results/", "isConcave_test1");
    // // geo_util::visualize({polygon2}, "../../tests/results/", "isConcave_test2");
    // // geo_util::visualize({polygon3}, "../../tests/results/", "isConcave_test3");
    // // geo_util::visualize({polygon4}, "../../tests/results/", "isConcave_test4");

    // bool a, b, c, d, e;
    // a = geo_util::isConcave(polygon1);
    // b = geo_util::isConcave(polygon2);
    // c = geo_util::isConcave(polygon3);
    // d = geo_util::isConcave(polygon4);
    // e = geo_util::isConcave(polygon5);

    // std::cout << a << " " << b << " " << c << " " << d << " " << e << "\n";
    // assert(a == false);
    // assert(b == false);
    // assert(c == true);
    // assert(d == false);
    // assert(e == true);
}

int main()
{
    is_concave_test();
    return 0;
}