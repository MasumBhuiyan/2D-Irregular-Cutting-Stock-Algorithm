#include <bin_packing.hpp>
#include <cassert>

void is_concave_test()
{
    Polygon polygon1, polygon2, polygon3, polygon4, polygon5, polygon6, polygon7, polygon8, polygon9, polygon10;
    boost_geo::read_wkt("POLYGON((0 0, 4 0, 1 -4, 1 -1, 0 -3, 0 0))", polygon1);
    boost_geo::read_wkt("POLYGON((0 0, 2 5, 2 2, 4 5, 4 2, 5 5, 6 2, 0 0))", polygon2);
    boost_geo::read_wkt("POLYGON((0 0, 0 5, 2 2, 5 0, 0 0))", polygon3);
    boost_geo::read_wkt("POLYGON((0 0, 3 2, 0 5, 3 8, 3 4, 7 4, 5 8, 10 8, 7 0, 0 0))", polygon4);
    boost_geo::read_wkt("POLYGON((0 0, 2 3, 0 3, 0 2, -4 2, 0 5, 5 5, 0 0))", polygon5);
    boost_geo::read_wkt("POLYGON((-2 0, -2 6, 2 7, 5 7, 8 5, 8 1, 4 0, -2 0))", polygon6);
    boost_geo::read_wkt("POLYGON((0 0, 0 5, 5 5, 5 0, 0 0))", polygon7);
    boost_geo::read_wkt("POLYGON((0 0, 0 5, 5 0, 0 0))", polygon8);
    boost_geo::read_wkt("POLYGON((0 0, -1 4, 2 0, 2 -4, 0 0))", polygon9);
    boost_geo::read_wkt("POLYGON((0 0, 2 3, 7 3, 5 0, 0 0))", polygon10);

    std::cout << "cluster_util::isConcave(): Is this a concave polygon?\n";
    std::cout << "Test  1: expected yes, found " << (geo_util::isConcave( polygon1) ==  true ? "yes" : " no") << "\n";
    std::cout << "Test  2: expected yes, found " << (geo_util::isConcave( polygon2) ==  true ? "yes" : " no") << "\n";
    std::cout << "Test  3: expected yes, found " << (geo_util::isConcave( polygon3) ==  true ? "yes" : " no") << "\n";
    std::cout << "Test  4: expected yes, found " << (geo_util::isConcave( polygon4) ==  true ? "yes" : " no") << "\n";
    std::cout << "Test  8: expected yes, found " << (geo_util::isConcave( polygon5) ==  true ? "yes" : " no") << "\n";
    std::cout << "Test  6: expected  no, found " << (geo_util::isConcave( polygon6) == false ? " no" : "yes") << "\n";
    std::cout << "Test  7: expected  no, found " << (geo_util::isConcave( polygon7) == false ? " no" : "yes") << "\n";
    std::cout << "Test  8: expected  no, found " << (geo_util::isConcave( polygon8) == false ? " no" : "yes") << "\n";
    std::cout << "Test  9: expected  no, found " << (geo_util::isConcave( polygon9) == false ? " no" : "yes") << "\n";
    std::cout << "Test 10: expected  no, found " << (geo_util::isConcave(polygon10) == false ? " no" : "yes") << "\n";

    assert
    ( 
        geo_util::isConcave(polygon1) ==  true &&
        geo_util::isConcave(polygon2) ==  true &&
        geo_util::isConcave(polygon3) ==  true &&
        geo_util::isConcave(polygon4) ==  true &&
        geo_util::isConcave(polygon5) ==  true &&
        geo_util::isConcave(polygon6) ==  false &&
        geo_util::isConcave(polygon7) ==  false &&
        geo_util::isConcave(polygon8) ==  false &&
        geo_util::isConcave(polygon9) ==  false &&
        geo_util::isConcave(polygon10) ==  false
    );
}
int main()
{
    is_concave_test();
    return 0;
}