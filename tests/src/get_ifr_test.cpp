#include <bin_packing.hpp>
#include <cassert>

void visualizer(Polygon polygon, double length, double width, std::string location, int testId)
{
    Box box;
    MultiPolygon multipolygon;
    Polygon poly;
    poly.outer().push_back(Point(0, 0));
    poly.outer().push_back(Point(0, length));
    poly.outer().push_back(Point(width, length));
    poly.outer().push_back(Point(width, 0));
    poly.outer().push_back(Point(0, 0));
    Polygon ifr = polygon_fit::getInnerFitRectangle({polygon}, length, width);

    for(auto point: ifr.outer())
    {
        Polygon p = geo_util::translatePolygon(polygon, point);
        multipolygon.push_back(p);
    }

    std::ostringstream name;
    name << "test" << std::setw(2) << std::setfill('0') << testId << ".svg";
    std::ofstream svg(location + name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 300, 300, "width=\"200mm\" height=\"200mm\" viewBox=\"-250 -250 800 800\"");
    mapper.add(poly);
    mapper.map(poly, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:2");
    mapper.add(ifr);
    mapper.map(ifr, "fill-opacity:0.5;fill:rgb(212, 0, 0);stroke:rgb(212,0,0);stroke-width:2");
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
}


void get_ifr_test()
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

    std::cout << "cluster_util::getInnerFitRectangle() visualization\n";
    visualizer( polygon1, 60, 40, "../../tests/results/get_ifr_test/",  1);
    visualizer( polygon2, 60, 40, "../../tests/results/get_ifr_test/",  2);
    visualizer( polygon3, 60, 40, "../../tests/results/get_ifr_test/",  3);
    visualizer( polygon4, 60, 40, "../../tests/results/get_ifr_test/",  4);
    visualizer( polygon5, 60, 40, "../../tests/results/get_ifr_test/",  5);
    visualizer( polygon6, 60, 40, "../../tests/results/get_ifr_test/",  6);
    visualizer( polygon7, 60, 40, "../../tests/results/get_ifr_test/",  7);
    visualizer( polygon8, 60, 40, "../../tests/results/get_ifr_test/",  8);
    visualizer( polygon9, 60, 40, "../../tests/results/get_ifr_test/",  9);
    visualizer(polygon10, 60, 40, "../../tests/results/get_ifr_test/", 10);
}   
int main()
{
    get_ifr_test();
    return 0;
}