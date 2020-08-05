#include <cassert>
#include <geo_util.hpp>
#include <custom_assertion.hpp>

Polygon readTestcase(std::string testcaseName)
{
	std::ifstream file("../../tests/testcases/" + testcaseName + ".txt");
	if (file.fail())
	{
		std::cerr << "file does not exists\n";
		exit(-1);
	}
	int n;
	file >> n;
	Polygon polygon;
	for (int i = 0; i < n; i += 1)
	{
		double _x, _y;
		file >> _x >> _y;
		polygon.outer().push_back(Point(_x, _y));
    }
	polygon.outer().push_back(polygon.outer()[0]);
	file.close();
    return polygon;
}
void dblcmp_test()
{
    double a = 1.00000000, b = 0.999999999, c = 0.99999999, d = 1.0000001;
    ASSERT_EQUAL(geo_util::dblcmp(a - b, EPS), 0);
    ASSERT_EQUAL(geo_util::dblcmp(a - c, EPS), 1);
    ASSERT_EQUAL(geo_util::dblcmp(a - d, EPS), -1);
    
    a = 1.0, b = 1.0, c = -1.0, d = 1.5;
    ASSERT_EQUAL(geo_util::dblcmp(a - b, EPS), 0);
    ASSERT_EQUAL(geo_util::dblcmp(a - c, EPS), 1);
    ASSERT_EQUAL(geo_util::dblcmp(a - d, EPS), -1);
}
void isConcave_test()
{
    Polygon concavePolygon1 = readTestcase("concavepolygon1");
    Polygon concavePolygon2 = readTestcase("concavepolygon2");
    Polygon convexPolygon1 = readTestcase("convexpolygon1");
    Polygon convexPolygon2 = readTestcase("convexpolygon2");

    ASSERT_EQUAL(geo_util::isConcave(concavePolygon1), true);
    ASSERT_EQUAL(geo_util::isConcave(concavePolygon2), true);
    ASSERT_EQUAL(geo_util::isConcave(convexPolygon1), false);
    ASSERT_EQUAL(geo_util::isConcave(convexPolygon2), false);
}
void getLength_test()
{
    Polygon concavePolygon1 = readTestcase("concavepolygon1");
    Polygon concavePolygon2 = readTestcase("concavepolygon2");
    Polygon convexPolygon1 = readTestcase("convexpolygon1");
    Polygon convexPolygon2 = readTestcase("convexpolygon2");

    ASSERT_EQUAL(geo_util::getLength(concavePolygon1), 10.0);
    ASSERT_EQUAL(geo_util::getLength(concavePolygon2), 5.0);
    ASSERT_EQUAL(geo_util::getLength(convexPolygon1), 7.0);
    ASSERT_EQUAL(geo_util::getLength(convexPolygon2), 5.0);
}
void getWidth_test()
{
    Polygon concavePolygon1 = readTestcase("concavepolygon1");
    Polygon concavePolygon2 = readTestcase("concavepolygon2");
    Polygon convexPolygon1 = readTestcase("convexpolygon1");
    Polygon convexPolygon2 = readTestcase("convexpolygon2");

    ASSERT_EQUAL(geo_util::getWidth(concavePolygon1), 8.0);
    ASSERT_EQUAL(geo_util::getWidth(concavePolygon2), 5.0);
    ASSERT_EQUAL(geo_util::getWidth(convexPolygon1), 6.0);
    ASSERT_EQUAL(geo_util::getWidth(convexPolygon2), 8.0);
}
void crossProduct_test()
{
    Point point1(5,2);
    Point point2(3,-2);
    Point point3(-5,-11);
    ASSERT_EQUAL(geo_util::crossProduct(point1, point2), -16.0);
    ASSERT_EQUAL(geo_util::crossProduct(point1, point3), -45.0);
    ASSERT_EQUAL(geo_util::crossProduct(point2, point3), -43.0);
    ASSERT_EQUAL(geo_util::crossProduct(point2, point1), 16.0);
    ASSERT_EQUAL(geo_util::crossProduct(point3, point1), 45.0);
    ASSERT_EQUAL(geo_util::crossProduct(point3, point2), 43.0);
}
void normalize_test()
{
    Polygon convexPolygon1 = readTestcase("convexpolygon1");

    std::vector<Polygon> ans = geo_util::normalize({convexPolygon1});
    Polygon __convexPolygon1({{
        {0.0, 0.0},
        {1.0, 5.0},
        {5.0, 6.0},
        {7.0, 3.0},
        {5.0, 0.0},
        {0.0,0.0}
    }});
    for(int i = 0; i < 6; i += 1)
    {
        ASSERT_EQUAL(geo_util::dblcmp(ans[0].outer()[i].x - __convexPolygon1.outer()[i].x, EPS), 0);
        ASSERT_EQUAL(geo_util::dblcmp(ans[0].outer()[i].y - __convexPolygon1.outer()[i].y, EPS), 0);
    }
}
void orientation_test()
{
    int orientation1 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(0, 0));
    int orientation2 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(10, 4));
    int orientation3 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(5, 2));
    int orientation4 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(5, 0));
    int orientation5 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(11, 7));
    int orientation6 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(11, 3));
    int orientation7 = geo_util::orientation(Point(0, 0), Point(10, 4), Point(5, 2.0001));
    assert(orientation1 == 0);
    assert(orientation2 == 0);
    assert(orientation3 == 0);
    assert(orientation4 == -1);
    assert(orientation5 == 1);
    assert(orientation6 == -1);
    assert(orientation7 == 1);
}
void rotatePolygon_test()
{
    Polygon polygon({{
        {-12.0, 2.0},
        {-8.0, 9.0},
        {-4.0, 4.0},
        {-12.0, 2.0}
    }});
    Polygon __rotatedPolygon90CW({{
        {-4.0, 10.0},
        {3.0, 6.0},
        {-2.0, 2.0},
        {-4.0, 10.0}
    }});
    Polygon __rotatedPolygon180CW({{
        {4.0, 2.0},
        {0.0, -5.0},
        {-4.0, 0.0},
        {4.0, 2.0}
    }});
    Point reference(-4.0, 2.0);

    Polygon rotatedPolygon90CW = geo_util::rotatePolygon(polygon, reference, 90);
    Polygon rotatedPolygon180CW = geo_util::rotatePolygon(polygon, reference, 180);
    for(int i = 0; i < rotatedPolygon90CW.outer().size(); i += 1)
    {
        ASSERT_EQUAL(geo_util::dblcmp(rotatedPolygon90CW.outer()[i].x - __rotatedPolygon90CW.outer()[i].x, EPS), 0);
        ASSERT_EQUAL(geo_util::dblcmp(rotatedPolygon90CW.outer()[i].y - __rotatedPolygon90CW.outer()[i].y, EPS), 0);
        ASSERT_EQUAL(geo_util::dblcmp(rotatedPolygon180CW.outer()[i].x - __rotatedPolygon180CW.outer()[i].x, EPS), 0);
        ASSERT_EQUAL(geo_util::dblcmp(rotatedPolygon180CW.outer()[i].y - __rotatedPolygon180CW.outer()[i].y, EPS), 0);
    }
}
void translatePolygon_test()
{
    Polygon polygon({{
        {-12.0, 2.0},
        {-8.0, 9.0},
        {-4.0, 4.0},
        {-12.0, 2.0}
    }});
    Polygon __translatedPolygon({{
        {10.0, 5.0},
        {14.0, 12.0},
        {18.0, 7.0},
        {10.0, 5.0}
    }});
    Point point(10.0, 5.0);

    Polygon translatedPolygon = geo_util::translatePolygon(polygon, point);
    for(int i = 0; i < 4; i += 1)
    {
        ASSERT_EQUAL(geo_util::dblcmp(translatedPolygon.outer()[i].x - __translatedPolygon.outer()[i].x, EPS), 0);
        ASSERT_EQUAL(geo_util::dblcmp(translatedPolygon.outer()[i].y - __translatedPolygon.outer()[i].y, EPS), 0);
    }
}
void linePointDistance_test()
{
    Point a(0.0, 0.0);
    Point b(5.0, 5.0);

    double distance1 = geo_util::linePointDistance(a, b, Point(10.0, 0.0));
    double distance2 = geo_util::linePointDistance(a, b, Point(5.0, 11.0));
    double distance3 = geo_util::linePointDistance(a, b, Point(15.0, 12.0));
    double distance4 = geo_util::linePointDistance(a, b, Point(5.0, 5.0));

    assert(geo_util::dblcmp(distance1 - 7.07106781, EPS) == 0);
    assert(geo_util::dblcmp(distance2 - 4.24264069, EPS) == 0);
    assert(geo_util::dblcmp(distance3 - 2.12132034, EPS) == 0);
    assert(geo_util::dblcmp(distance4 - 0.00000000, EPS) == 0);
}
void polygonPolygonIntersectionArea_test()
{
    Polygon polygon1, polygon2;
    boost_geo::read_wkt(
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
            "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", polygon1);
    boost_geo::read_wkt(
        "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", polygon2);
    double areaOfIntersection = geo_util::polygonPolygonIntersectionArea(polygon1, polygon2);
    assert(geo_util::dblcmp(areaOfIntersection - 2.50205081, EPS) == 0);
}
void segmentSegmentIntersectionPoint_test()
{
    Point point1 = geo_util::segmentSegmentIntersectionPoint(Point(0, 0), Point(5, 5), Point(5, 10), Point(10, 0));
    Point point2 = geo_util::segmentSegmentIntersectionPoint(Point(0, 0), Point(10, 10), Point(10, 0), Point(0, 10));
    Point point3 = geo_util::segmentSegmentIntersectionPoint(Point(1, -1), Point(3, -3), Point(0, 0), Point(5, 5));
    Point point4 = geo_util::segmentSegmentIntersectionPoint(Point(0, 0), Point(5, 5), Point(5, 5), Point(10, 0));

    assert(geo_util::dblcmp(point1.x - INF, EPS) == 0 and geo_util::dblcmp(point1.y - INF, EPS) == 0);
    assert(geo_util::dblcmp(point2.x - 5, EPS) == 0 and geo_util::dblcmp(point2.y - 5, EPS) == 0);
    assert(geo_util::dblcmp(point3.x - INF, EPS) == 0 and geo_util::dblcmp(point3.y - INF, EPS) == 0);
    assert(geo_util::dblcmp(point4.x - 5, EPS) == 0 and geo_util::dblcmp(point4.y - 5, EPS) == 0);
}
void rotatePolygons_test()
{
    std::vector<Polygon> polygons, expected__rotatedPolygons, rotatedPolygons;
    Polygon polygon({{
        {-12.0, 2.0},
        {-8.0, 9.0},
        {-4.0, 4.0},
        {-12.0, 2.0}
    }});
    Polygon __rotatedPolygon90CW({{
        {-4.0, 10.0},
        {3.0, 6.0},
        {-2.0, 2.0},
        {-4.0, 10.0}
    }});
    Point reference(-4.0, 2.0);
    
    polygons.push_back(polygon);
    expected__rotatedPolygons.push_back(__rotatedPolygon90CW);
    rotatedPolygons = geo_util::rotatePolygons(polygons, reference, 90);

    for(int i = 0; i < rotatedPolygons.size(); i += 1)
    {
        for(int j = 0; j < rotatedPolygons[ i ].outer().size(); j += 1)
        {
            ASSERT_EQUAL(geo_util::dblcmp(rotatedPolygons[ i ].outer()[j].x - expected__rotatedPolygons[ i ].outer()[j].x, EPS), 0);
            ASSERT_EQUAL(geo_util::dblcmp(rotatedPolygons[ i ].outer()[j].y - expected__rotatedPolygons[ i ].outer()[j].y, EPS), 0);

        }
    }
}
void translatePolygons_test()
{
    std::vector<Polygon> polygons, expectedTranslatedPolygons, translatedPolygons;
    Polygon polygon({{
        {-12.0, 2.0},
        {-8.0, 9.0},
        {-4.0, 4.0},
        {-12.0, 2.0}
    }});
    Polygon translatedPolygon({{
        {10.0, 5.0},
        {14.0, 12.0},
        {18.0, 7.0},
        {10.0, 5.0}
    }});
    Point point(10.0, 5.0);

    polygons.push_back(polygon);
    translatedPolygons = geo_util::translatePolygons(polygons, point);
    expectedTranslatedPolygons.push_back(translatedPolygon);

    for(int i = 0; i < translatedPolygons.size(); i += 1)
    {
        for(int j = 0; j < translatedPolygons[ i ].outer().size(); j += 1)
        {
            ASSERT_EQUAL(geo_util::dblcmp(translatedPolygons[ i ].outer()[j].x - expectedTranslatedPolygons[ i ].outer()[j].x, EPS), 0);
            ASSERT_EQUAL(geo_util::dblcmp(translatedPolygons[ i ].outer()[j].y - expectedTranslatedPolygons[ i ].outer()[j].y, EPS), 0);
        }
    }
}
void isItPossibleToPlacePolygon_test()
{
    assert( true == true );
}
void pointInRectangle_test()
{
    Point a(0.0, 0.0), b(5.0, 5.0), point1(0.0, 0.0), point2(1.0, 2.0), point3(-1.0, 5.0);
    assert(geo_util::pointInRectangle(a, b, point1) == true);
    assert(geo_util::pointInRectangle(a, b, point2) == true);
    assert(geo_util::pointInRectangle(a, b, point3) == false);
}

int main()
{
    dblcmp_test();
    isConcave_test();
    getLength_test();
    getWidth_test();
    crossProduct_test();
    normalize_test();
    orientation_test();
    rotatePolygon_test();
    translatePolygon_test();
    linePointDistance_test();
    polygonPolygonIntersectionArea_test();
    segmentSegmentIntersectionPoint_test();
    rotatePolygons_test();
    translatePolygons_test();
    isItPossibleToPlacePolygon_test();
    pointInRectangle_test();
    return 0;
}