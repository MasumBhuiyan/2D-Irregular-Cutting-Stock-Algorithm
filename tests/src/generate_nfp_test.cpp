#include <bin_packing.hpp>

void visualizer(Polygon polygon, MultiPolygon cluster, Polygon nfp, std::string location, int testId)
{
    Box box;
    MultiPolygon multipolygon;
    for(auto point: nfp.outer())
    {
        MultiPolygon polygons = geo_util::translatePolygons(cluster, point);     
        for(auto poly: polygons)
        {
            multipolygon.push_back(poly);
        }   
    }
    for(auto innerPoly: nfp.inners())
    {
        for(auto point: innerPoly)
        {
            MultiPolygon polygons = geo_util::translatePolygons(cluster, point);     
            for(auto poly: polygons)
            {
                multipolygon.push_back(poly);
            }   
        }
    }
    std::ostringstream name;
    name << "test" << std::setw(2) << std::setfill('0') << testId << ".svg";
    std::ofstream svg(location + name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 300, 300, "width=\"200mm\" height=\"200mm\" viewBox=\"-250 -250 800 800\"");
    mapper.add(nfp);
    mapper.map(nfp, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:2");
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(212, 0, 0);stroke:rgb(212,0,0);stroke-width:2");
    mapper.add(polygon);
    mapper.map(polygon, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
}

void generate_nfp_test()
{
    Polygon polygon1, polygon2, polygon3, polygon4, polygon5, polygon6, polygon7, polygon8, polygon9, polygon10;
    Polygon polygon11, polygon12, polygon13;
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
    boost_geo::read_wkt("POLYGON((5 0, 5 5, 10 0, 5 0))", polygon11);
    boost_geo::read_wkt("POLYGON((24.4615 26.6154,35.4615 26.6154,36.4615 28.6154,35.4615 29.6154,35.4615 31.6154,34.4615 32.6154,30.4615 31.6154,27.4615 32.6154,24.4615 31.6154,23.4615 29.6154,24.4615 26.6154))", polygon12);
    boost_geo::read_wkt("POLYGON((2 0,9 1,9 5,2 7,1 5,1 4,0 3,1 2,2 0))", polygon13);

    Polygon  nfp1 = polygon_fit::getNoFitPolygon(polygon1, { polygon2});
    Polygon  nfp2 = polygon_fit::getNoFitPolygon(polygon1, { polygon3});
    Polygon  nfp3 = polygon_fit::getNoFitPolygon(polygon2, { polygon4});
    Polygon  nfp4 = polygon_fit::getNoFitPolygon(polygon2, { polygon5});
    Polygon  nfp5 = polygon_fit::getNoFitPolygon(polygon3, { polygon6});
    Polygon  nfp6 = polygon_fit::getNoFitPolygon(polygon3, { polygon7});
    Polygon  nfp7 = polygon_fit::getNoFitPolygon(polygon4, { polygon8});
    Polygon  nfp8 = polygon_fit::getNoFitPolygon(polygon4, { polygon9});
    Polygon  nfp9 = polygon_fit::getNoFitPolygon(polygon5, { polygon7});
    Polygon nfp10 = polygon_fit::getNoFitPolygon(polygon5, {polygon10});
    Polygon nfp11 = polygon_fit::getNoFitPolygon(polygon1, {polygon7,polygon11});
    Polygon nfp12 = polygon_fit::getNoFitPolygon(polygon2, {polygon7,polygon11});
    Polygon nfp13 = polygon_fit::getNoFitPolygon(polygon3, {polygon7,polygon11});
    Polygon nfp14 = polygon_fit::getNoFitPolygon(polygon6, {polygon7,polygon11});
    Polygon nfp15 = polygon_fit::getNoFitPolygon(polygon8, {polygon7,polygon11});
    Polygon nfp16 = polygon_fit::getNoFitPolygon(polygon12, {polygon13});

    std::cout << "cluster_util::getNoFitPolygon() visualization\n";
    visualizer(polygon1, { polygon2},  nfp1, "../../tests/results/generate_nfp_test/",  1);
    visualizer(polygon1, { polygon3},  nfp2, "../../tests/results/generate_nfp_test/",  2);
    visualizer(polygon2, { polygon4},  nfp3, "../../tests/results/generate_nfp_test/",  3);
    visualizer(polygon2, { polygon5},  nfp4, "../../tests/results/generate_nfp_test/",  4);
    visualizer(polygon3, { polygon6},  nfp5, "../../tests/results/generate_nfp_test/",  5);
    visualizer(polygon3, { polygon7},  nfp6, "../../tests/results/generate_nfp_test/",  6);
    visualizer(polygon4, { polygon8},  nfp7, "../../tests/results/generate_nfp_test/",  7);
    visualizer(polygon4, { polygon9},  nfp8, "../../tests/results/generate_nfp_test/",  8);
    visualizer(polygon5, { polygon7},  nfp9, "../../tests/results/generate_nfp_test/",  9);
    visualizer(polygon5, {polygon10}, nfp10, "../../tests/results/generate_nfp_test/", 10);

    visualizer(polygon1,  {polygon7,polygon11}, nfp11, "../../tests/results/generate_nfp_test/", 11);
    visualizer(polygon2,  {polygon7,polygon11}, nfp12, "../../tests/results/generate_nfp_test/", 12);
    visualizer(polygon3,  {polygon7,polygon11}, nfp13, "../../tests/results/generate_nfp_test/", 13);
    visualizer(polygon6,  {polygon7,polygon11}, nfp14, "../../tests/results/generate_nfp_test/", 14);
    visualizer(polygon8,  {polygon7,polygon11}, nfp15, "../../tests/results/generate_nfp_test/", 15);
    visualizer(polygon12, {polygon13}, nfp16, "../../tests/results/generate_nfp_test/", 16);
}

int main()
{
    generate_nfp_test();
    return 0;
}