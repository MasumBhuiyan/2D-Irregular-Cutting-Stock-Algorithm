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
    Polygon concavePolygon1 = readTestcase("concavepolygon1");
    Polygon concavePolygon2 = readTestcase("concavepolygon2");
    Polygon convexPolygon1 = readTestcase("convexpolygon1");
    Polygon convexPolygon2 = readTestcase("convexpolygon2");

    geo_util::normalize(concavePolygon1);
    geo_util::normalize(concavePolygon2);
    geo_util::normalize(convexPolygon1);
    geo_util::normalize(convexPolygon2);

    for(auto point: concavePolygon1.outer()) assert(point.x >= 0 and point.y >= 0);
    for(auto point: concavePolygon2.outer()) assert(point.x >= 0 and point.y >= 0);
    for(auto point: convexPolygon1.outer()) assert(point.x >= 0 and point.y >= 0);
    for(auto point: convexPolygon2.outer()) assert(point.x >= 0 and point.y >= 0);
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
    
}
void translatePolygon_test()
{

}
void linePointDistance_test()
{

}
void polygonPolygonIntersectionArea_test()
{

}
void segmentSegmentIntersectionPoint_test()
{

}
void rotatePolygons_test()
{

}
void translatePolygons_test()
{

}
void isItPossibleToPlacePolygon_test()
{

}
void pointInRectangle_test()
{

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