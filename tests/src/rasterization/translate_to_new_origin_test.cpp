#include "rasterization.hpp"
#include "custom_assertion.hpp"

void translate_to_new_origin_test1()
{
	double max_x = -MAX_X;
    double max_y = -MAX_Y;
    Point seed = Point(7,5);
    Polygon polygon
    ({
    	Point(2, 6),
    	Point(7, 1),
    	Point(6, 4),
    	Point(10, 4),
    	Point(7, 3),
    	Point(11, 2),
    	Point(11, 8 ),
    	Point(9, 5),
    	Point(6, 10),
    	Point(6, 6)
    });

    Polygon expectedPolygon
    ({
    	Point(4,0),
    	Point(9,5),
    	Point(6,4),
    	Point(6,8),
    	Point(7,5),
    	Point(8,9),
    	Point(2,9),
    	Point(5,7),
    	Point(0,4),
    	Point(4,4)
    });
    raster::translateToNewOrigin(polygon, max_x, max_y, seed);
    ASSERT_EQUAL(polygon, expectedPolygon);
    ASSERT_EQUAL(seed, Point(5,5));
}

void translate_to_new_origin_test2()
{
    double max_x = -MAX_X;
    double max_y = -MAX_Y;
    Point seed = Point(10,10);
    
    Polygon polygon
    ({
        Point(0,0),
        Point(15,0),
        Point(15,12),
        Point(14,12),
        Point(13,13),
        Point(13,15),
        Point(15,18),
        Point(10,20),
        Point(9,19),
        Point(6,19),
        Point(5,20),
        Point(0,18),
        Point(2,15),
        Point(2,13),
        Point(1,12),
        Point(0,12)
    });
    Polygon expectedPolygon
    ({
        Point(20,0),
        Point(20,15),
        Point(8,15),
        Point(8,14),
        Point(7,13),
        Point(5,13),
        Point(2,15),
        Point(0,10),
        Point(1,9),
        Point(1,6),
        Point(0,5),
        Point(2,0),
        Point(5,2),
        Point(7,2),
        Point(8,1),
        Point(8,0)
    });
    raster::translateToNewOrigin(polygon, max_x, max_y, seed);
    ASSERT_EQUAL(polygon, expectedPolygon);
    ASSERT_EQUAL(seed, Point(10,10));
}
int main()
{
    translate_to_new_origin_test1();
    translate_to_new_origin_test2();
}