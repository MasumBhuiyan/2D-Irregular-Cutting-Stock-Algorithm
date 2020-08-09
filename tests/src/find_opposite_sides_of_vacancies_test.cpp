#include <bin_packing.hpp>
#include <cassert>

void visualizer(Polygon polygon, std::string location, int testId)
{
    std::cout << " Test " << testId << ": ";
    if( geo_util::isAConcavePolygon(polygon) == false )
    {
        std::cout << "It is a convex polygon. So convex hull vacancy is null.\n";
    }
    else
    {
        Polygon hull = cluster_util::convexHull({polygon});
        MultiPolygon vacancies = cluster_util::findConvexHullVacancy(polygon);
        vector<tuple<Point,Point>> oppositeSidesOfVacancy = cluster_util::findOppositeSideOfVacancies(hull, vacancies);
        MultiPolygon multipolygon;
        multipolygon.push_back(polygon);
        Box box;
	    boost::geometry::envelope(multipolygon, box);
        std::cout << "It is a concave polygon. So convex hull vacancies are presented in the ../../tests/results/ folder.\n";
        std::ostringstream name;
        name << "test" << std::setw(2) << std::setfill('0') << testId << ".svg";
        std::ofstream svg(location + name.str());
        boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
        mapper.add(multipolygon);
        mapper.add(vacancies);
        mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
        mapper.map(vacancies, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:2");
        mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(153,204,0);stroke-width:2;stroke-dasharray:1,7;stroke-linecap:round");
        
        for(auto line: oppositeSidesOfVacancy)
        {
            Point a, b;
            std::tie(a, b) = line;
            Line linestring;
            linestring.push_back(a);
            linestring.push_back(b);
            mapper.add(linestring);
            mapper.map(linestring, "opacity:0.5;fill:none;stroke:rgb(0,0,0);stroke-width:2");
        }
    }
}

void find_opposite_sides_of_vacancies_test()
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

     Polygon  hull1 = cluster_util::convexHull( {polygon1});
     Polygon  hull2 = cluster_util::convexHull( {polygon2});
     Polygon  hull3 = cluster_util::convexHull( {polygon3});
     Polygon  hull4 = cluster_util::convexHull( {polygon4});
     Polygon  hull5 = cluster_util::convexHull( {polygon5});
     Polygon  hull6 = cluster_util::convexHull( {polygon6});
     Polygon  hull7 = cluster_util::convexHull( {polygon7});
     Polygon  hull8 = cluster_util::convexHull( {polygon8});
     Polygon  hull9 = cluster_util::convexHull( {polygon9});
     Polygon hull10 = cluster_util::convexHull({polygon10});

    MultiPolygon  vacancies1 = cluster_util::findConvexHullVacancy( polygon1);
    MultiPolygon  vacancies2 = cluster_util::findConvexHullVacancy( polygon2);
    MultiPolygon  vacancies3 = cluster_util::findConvexHullVacancy( polygon3);
    MultiPolygon  vacancies4 = cluster_util::findConvexHullVacancy( polygon4);
    MultiPolygon  vacancies5 = cluster_util::findConvexHullVacancy( polygon5);
    MultiPolygon  vacancies6 = cluster_util::findConvexHullVacancy( polygon6);
    MultiPolygon  vacancies7 = cluster_util::findConvexHullVacancy( polygon7);
    MultiPolygon  vacancies8 = cluster_util::findConvexHullVacancy( polygon8);
    MultiPolygon  vacancies9 = cluster_util::findConvexHullVacancy( polygon9);
    MultiPolygon vacancies10 = cluster_util::findConvexHullVacancy(polygon10);

    vector<tuple<Point,Point>>  oppositeSidesOfVacancy1 = cluster_util::findOppositeSideOfVacancies( hull1,  vacancies1);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy2 = cluster_util::findOppositeSideOfVacancies( hull2,  vacancies2);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy3 = cluster_util::findOppositeSideOfVacancies( hull3,  vacancies3);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy4 = cluster_util::findOppositeSideOfVacancies( hull4,  vacancies4);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy5 = cluster_util::findOppositeSideOfVacancies( hull5,  vacancies5);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy6 = cluster_util::findOppositeSideOfVacancies( hull6,  vacancies6);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy7 = cluster_util::findOppositeSideOfVacancies( hull7,  vacancies7);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy8 = cluster_util::findOppositeSideOfVacancies( hull8,  vacancies8);
    vector<tuple<Point,Point>>  oppositeSidesOfVacancy9 = cluster_util::findOppositeSideOfVacancies( hull9,  vacancies9);
    vector<tuple<Point,Point>> oppositeSidesOfVacancy10 = cluster_util::findOppositeSideOfVacancies(hull10, vacancies10);

    assert
    (
         vacancies1.size() ==  oppositeSidesOfVacancy1.size() &&
         vacancies2.size() ==  oppositeSidesOfVacancy2.size() &&
         vacancies3.size() ==  oppositeSidesOfVacancy3.size() &&
         vacancies4.size() ==  oppositeSidesOfVacancy4.size() &&
         vacancies5.size() ==  oppositeSidesOfVacancy5.size() &&
         vacancies6.size() ==  oppositeSidesOfVacancy6.size() &&
         vacancies7.size() ==  oppositeSidesOfVacancy7.size() &&
         vacancies8.size() ==  oppositeSidesOfVacancy8.size() &&
         vacancies9.size() ==  oppositeSidesOfVacancy9.size() &&
        vacancies10.size() == oppositeSidesOfVacancy10.size() 
    );

    std::cout << "cluster_util::findConvexHullVacancy() visualization\n";
    visualizer( polygon1, "../../tests/results/find_opposite_sides_of_vacancies_test/",  1);
    visualizer( polygon2, "../../tests/results/find_opposite_sides_of_vacancies_test/",  2);
    visualizer( polygon3, "../../tests/results/find_opposite_sides_of_vacancies_test/",  3);
    visualizer( polygon4, "../../tests/results/find_opposite_sides_of_vacancies_test/",  4);
    visualizer( polygon5, "../../tests/results/find_opposite_sides_of_vacancies_test/",  5);
    visualizer( polygon6, "../../tests/results/find_opposite_sides_of_vacancies_test/",  6);
    visualizer( polygon7, "../../tests/results/find_opposite_sides_of_vacancies_test/",  7);
    visualizer( polygon8, "../../tests/results/find_opposite_sides_of_vacancies_test/",  8);
    visualizer( polygon9, "../../tests/results/find_opposite_sides_of_vacancies_test/",  9);
    visualizer(polygon10, "../../tests/results/find_opposite_sides_of_vacancies_test/", 10);
}
int main()
{
    find_opposite_sides_of_vacancies_test();
    return 0;
}