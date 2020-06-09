#include "rasterization.hpp"
#include "custom_assertion.hpp"

void get_raster_matrix_dimension_test1()
{
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

	pair<int, int> row_col = raster::getRasterMatrixDimension(polygon);
	ASSERT_EQUAL(row_col, pair<int, int>(11, 11));
}

int main()
{
	get_raster_matrix_dimension_test1();
}