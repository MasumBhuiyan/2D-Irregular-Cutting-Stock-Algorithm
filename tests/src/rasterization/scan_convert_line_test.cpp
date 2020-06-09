#include "rasterization.hpp"
#include "Matrix.hpp"
#include "custom_assertion.hpp"

void scan_convert_line_test1()
{
	Matrix matrix(101, 101);

	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 90));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 80));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 70));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 60));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 50));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 40));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 30));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 20));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 10));
	raster::scanConvertLine(matrix, Point(50, 50), Point(100, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(90, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(80, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(70, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(60, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(50, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(40, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(30, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(20, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(10, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 0));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 10));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 20));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 30));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 40));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 50));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 60));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 70));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 80));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 90));
	raster::scanConvertLine(matrix, Point(50, 50), Point(0, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(10, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(20, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(30, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(40, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(50, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(60, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(70, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(80, 100));
	raster::scanConvertLine(matrix, Point(50, 50), Point(90, 100));

	matrix.print();
}

int main()
{
	scan_convert_line_test1();
}