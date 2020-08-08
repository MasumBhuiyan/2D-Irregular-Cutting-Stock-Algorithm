#include <bin_packing.hpp>

void generate_nfp_test()
{
    Polygon polygon1({{
        {0,0},
        {0,5},
        {5,5},
        {5, 0},
        {0,0}
    }});

    Polygon polygon2({{
        {0,0},
        {0,5},
        {5,5},
        {0,0}
    }});
    Polygon polygon3({{
        {0,0},
        {5,5},
        {5,0},
        {0,0}
    }});
    Polygon polygon4, polygon5;
    boost_geo::read_wkt("POLYGON((5 5,5 0,0 0,0 5,5 5))", polygon4);
    boost_geo::read_wkt("POLYGON((5 0,0 5,5 5,5 0))", polygon5);
    MultiPolygon nfp = polygon_fit::getNoFitPolygon(polygon4, {polygon5});

    for(auto &polygon: nfp)
    {
        assert(polygon.outer().size());
        std::cout << "polygon:\n";
        for(auto &point: polygon.outer())
        {
            std::cout << boost_geo::wkt(point) << "\n"; 
        }
    }
}

int main()
{
    generate_nfp_test();
    return 0;
}