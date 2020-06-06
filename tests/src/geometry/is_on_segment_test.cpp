#include "geometry.hpp"
#include "custom_assertion.hpp"

void is_on_segment_test1()
{
    Point a(0, 0), b(10, 5);
    Point p(4, 2);
    ASSERT_EQUAL(linear::isOnSegment(p, a, b), true);
    ASSERT_NOT_EQUAL(linear::isOnSegment(Point(4, 1.99), a, b), true);
    ASSERT_NOT_EQUAL(linear::isOnSegment(Point(0, 0.000345), a, b), true);
}

int main()
{
    is_on_segment_test1();
}