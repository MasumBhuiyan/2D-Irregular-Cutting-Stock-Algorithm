#include "geometry.hpp"
#include "custom_assertion.hpp"

void is_point_inside_polygon_test1()
{
    Polygon p({{0, 0},
               {2, -10},
               {6, -6},
               {13, -12},
               {15, 0},
               {10, 5}});

    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(5, 2), p), 1);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(5, 3.7452), p), -1);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(13, -12), p), 0);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(12, 3), p), 0);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(12, 3.000012), p), -1);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(12, 2.9997), p), 1);
}

void is_point_inside_polygon_test2()
{
    Polygon p({{0, 1},
               {5, -10},
               {3, -3},
               {5, -7},
               {6, -4},
               {8, -4}});

    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(4, 4.5), p), -1);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(4, 3.25), p), 1);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(6, 5), p), 1);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(7, -5), p), 0);
    ASSERT_EQUAL(polygonal::isPointInsidePolygon(Point(0, 2), p), -1);
}

int main()
{
    is_point_inside_polygon_test1();
}