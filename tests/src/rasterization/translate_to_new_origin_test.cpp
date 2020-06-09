#include "rasterization.hpp"
#include "custom_assertion.hpp"

void translate_to_new_origin_test1()
{
	double max_x = -MAX_X;
	double max_y = -MAX_Y;
	Polygon polygon({Point(2, 6),
					 Point(7, 1),
					 Point(6, 4),
					 Point(10, 4),
					 Point(7, 3),
					 Point(11, 2),
					 Point(11, 8),
					 Point(9, 5),
					 Point(6, 10),
					 Point(6, 6)});

	Polygon expectedPolygon({Point(0, 5),
							 Point(5, 0),
							 Point(4, 3),
							 Point(8, 3),
							 Point(5, 2),
							 Point(9, 1),
							 Point(9, 7),
							 Point(7, 4),
							 Point(4, 9),
							 Point(4, 5)});
	raster::translateToNewOrigin(polygon, max_x, max_y);
	ASSERT_EQUAL(polygon, expectedPolygon);
}

int main()
{
	translate_to_new_origin_test1();
}