#include <bin_packing.hpp>
#include <cassert>

void visualizer(Polygon polygon, std::string location, int testId)
{
    std::cout << " Test " << testId << ": ";
    if( geo_util::isConcave(polygon) == false )
    {
        std::cout << "It is a convex polygon. So convex hull vacancy is null.\n";
    }
    else
    {
        MultiPolygon multipolygon;
        multipolygon.push_back(polygon);
        MultiPolygon vacancies = cluster_util::findConvexHullVacancy(polygon);
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
    }
}

void find_convex_hull_vacancies_test()
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


    double  hullArea1 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon1})));
    double  hullArea2 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon2})));
    double  hullArea3 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon3})));
    double  hullArea4 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon4})));
    double  hullArea5 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon5})));
    double  hullArea6 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon6})));
    double  hullArea7 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon7})));
    double  hullArea8 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon8})));
    double  hullArea9 = std::fabs(boost_geo::area( cluster_util::convexHull({polygon9})));
    double hullArea10 = std::fabs(boost_geo::area(cluster_util::convexHull({polygon10})));

    double  polygonArea1 = std::fabs(boost_geo::area( polygon1));
    double  polygonArea2 = std::fabs(boost_geo::area( polygon2));
    double  polygonArea3 = std::fabs(boost_geo::area( polygon3));
    double  polygonArea4 = std::fabs(boost_geo::area( polygon4));
    double  polygonArea5 = std::fabs(boost_geo::area( polygon5));
    double  polygonArea6 = std::fabs(boost_geo::area( polygon6));
    double  polygonArea7 = std::fabs(boost_geo::area( polygon7));
    double  polygonArea8 = std::fabs(boost_geo::area( polygon8));
    double  polygonArea9 = std::fabs(boost_geo::area( polygon9));
    double polygonArea10 = std::fabs(boost_geo::area(polygon10));

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

    double  vacanciesArea1 = std::fabs(boost_geo::area( vacancies1));
    double  vacanciesArea2 = std::fabs(boost_geo::area( vacancies2));
    double  vacanciesArea3 = std::fabs(boost_geo::area( vacancies3));
    double  vacanciesArea4 = std::fabs(boost_geo::area( vacancies4));
    double  vacanciesArea5 = std::fabs(boost_geo::area( vacancies5));
    double  vacanciesArea6 = std::fabs(boost_geo::area( vacancies6));
    double  vacanciesArea7 = std::fabs(boost_geo::area( vacancies7));
    double  vacanciesArea8 = std::fabs(boost_geo::area( vacancies8));
    double  vacanciesArea9 = std::fabs(boost_geo::area( vacancies9));
    double vacanciesArea10 = std::fabs(boost_geo::area(vacancies10));

    std::cout << "cluster_util::findConvexHullVacancy() test\n";
    std::cout << "Test  1:  polygonArea1 +  vacanciesArea1 -  hullArea1: expected 0, found " <<  polygonArea1 +  vacanciesArea1 -  hullArea1 << "\n";  
    std::cout << "Test  2:  polygonArea2 +  vacanciesArea2 -  hullArea2: expected 0, found " <<  polygonArea2 +  vacanciesArea2 -  hullArea2 << "\n";  
    std::cout << "Test  3:  polygonArea3 +  vacanciesArea3 -  hullArea3: expected 0, found " <<  polygonArea3 +  vacanciesArea3 -  hullArea3 << "\n";  
    std::cout << "Test  4:  polygonArea4 +  vacanciesArea4 -  hullArea4: expected 0, found " <<  polygonArea4 +  vacanciesArea4 -  hullArea4 << "\n";  
    std::cout << "Test  5:  polygonArea5 +  vacanciesArea5 -  hullArea5: expected 0, found " <<  polygonArea5 +  vacanciesArea5 -  hullArea5 << "\n";  
    std::cout << "Test  6:  polygonArea6 +  vacanciesArea6 -  hullArea6: expected 0, found " <<  polygonArea6 +  vacanciesArea6 -  hullArea6 << "\n";  
    std::cout << "Test  7:  polygonArea7 +  vacanciesArea7 -  hullArea7: expected 0, found " <<  polygonArea7 +  vacanciesArea7 -  hullArea7 << "\n";  
    std::cout << "Test  8:  polygonArea8 +  vacanciesArea8 -  hullArea8: expected 0, found " <<  polygonArea8 +  vacanciesArea8 -  hullArea8 << "\n";  
    std::cout << "Test  9:  polygonArea9 +  vacanciesArea9 -  hullArea9: expected 0, found " <<  polygonArea9 +  vacanciesArea9 -  hullArea9 << "\n";  
    std::cout << "Test 10: polygonArea10 + vacanciesArea10 - hullArea10: expected 0, found " << polygonArea10 + vacanciesArea10 - hullArea10 << "\n";  

    assert
    (
        geo_util::dblcmp(( polygonArea1 +  vacanciesArea1 - hullArea1)) == 0 && 
        geo_util::dblcmp(( polygonArea2 +  vacanciesArea2 - hullArea2)) == 0 && 
        geo_util::dblcmp(( polygonArea3 +  vacanciesArea3 - hullArea3)) == 0 && 
        geo_util::dblcmp(( polygonArea4 +  vacanciesArea4 - hullArea4)) == 0 && 
        geo_util::dblcmp(( polygonArea5 +  vacanciesArea5 - hullArea5)) == 0 && 
        geo_util::dblcmp(( polygonArea6 +  vacanciesArea6 - hullArea6)) == 0 && 
        geo_util::dblcmp(( polygonArea7 +  vacanciesArea7 - hullArea7)) == 0 && 
        geo_util::dblcmp(( polygonArea8 +  vacanciesArea8 - hullArea8)) == 0 && 
        geo_util::dblcmp(( polygonArea9 +  vacanciesArea9 - hullArea9)) == 0 && 
        geo_util::dblcmp((polygonArea10 + vacanciesArea10 - hullArea10)) == 0 
    );

    std::cout << "cluster_util::findConvexHullVacancy() visualization\n";
    visualizer( polygon1, "../../tests/results/find_convex_hull_vacancies_test/",  1);
    visualizer( polygon2, "../../tests/results/find_convex_hull_vacancies_test/",  2);
    visualizer( polygon3, "../../tests/results/find_convex_hull_vacancies_test/",  3);
    visualizer( polygon4, "../../tests/results/find_convex_hull_vacancies_test/",  4);
    visualizer( polygon5, "../../tests/results/find_convex_hull_vacancies_test/",  5);
    visualizer( polygon6, "../../tests/results/find_convex_hull_vacancies_test/",  6);
    visualizer( polygon7, "../../tests/results/find_convex_hull_vacancies_test/",  7);
    visualizer( polygon8, "../../tests/results/find_convex_hull_vacancies_test/",  8);
    visualizer( polygon9, "../../tests/results/find_convex_hull_vacancies_test/",  9);
    visualizer(polygon10, "../../tests/results/find_convex_hull_vacancies_test/", 10);
}
int main()
{
    find_convex_hull_vacancies_test();
    return 0;
}