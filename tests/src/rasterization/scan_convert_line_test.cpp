#include "rasterization.hpp"
#include "Matrix.hpp"
#include "custom_assertion.hpp"

void scan_convert_line_test1()
{
	Matrix matrix1(11,11), matrix2(11, 11), expected_matrix(11,11);

	expected_matrix.mat[3][2] = 1;
	expected_matrix.mat[4][3] = 1;
	expected_matrix.mat[5][3] = 1;
	expected_matrix.mat[6][4] = 1;
	expected_matrix.mat[7][5] = 1;
	expected_matrix.mat[8][5] = 1;
	expected_matrix.mat[9][6] = 1;
	
	raster::scanConvertLine(matrix1, Point(3,2), Point(9,6));
	raster::scanConvertLine(matrix2, Point(9,6), Point(3,2));
	ASSERT_EQUAL(matrix1.mat, expected_matrix.mat);
	ASSERT_EQUAL(matrix2.mat, expected_matrix.mat);
}
void scan_convert_line_test2()
{
	Matrix matrix1(11,11), matrix2(11, 11), expected_matrix(11,11);

	expected_matrix.mat[2][9] = 1;
	expected_matrix.mat[3][8] = 1;
	expected_matrix.mat[4][7] = 1;
	expected_matrix.mat[5][6] = 1;
	expected_matrix.mat[6][5] = 1;
	
	raster::scanConvertLine(matrix1, Point(2,9), Point(6,3));
	raster::scanConvertLine(matrix2, Point(6,3), Point(2,9));
	ASSERT_EQUAL(matrix1.mat, expected_matrix.mat);
	ASSERT_EQUAL(matrix2.mat, expected_matrix.mat);
}
void scan_convert_line_test3()
{
	Matrix matrix1(11,11), matrix2(11, 11), expected_matrix(11,11);

	expected_matrix.mat[2][3] = 1;
	expected_matrix.mat[3][3] = 1;
	expected_matrix.mat[4][3] = 1;
	expected_matrix.mat[5][3] = 1;
	expected_matrix.mat[6][3] = 1;
	expected_matrix.mat[7][3] = 1;
	expected_matrix.mat[8][3] = 1;

	raster::scanConvertLine(matrix1, Point(2,3), Point(8,3));
	raster::scanConvertLine(matrix2, Point(8,3), Point(2,3));

	ASSERT_EQUAL(matrix1.mat, expected_matrix.mat);
	ASSERT_EQUAL(matrix2.mat, expected_matrix.mat);
}
void scan_convert_line_test4()
{
	Matrix matrix1(11,11), matrix2(11, 11), expected_matrix(11,11);

	expected_matrix.mat[1][1] = 1;
	expected_matrix.mat[1][2] = 1;
	expected_matrix.mat[1][3] = 1;
	expected_matrix.mat[1][4] = 1;
	expected_matrix.mat[1][5] = 1;
	expected_matrix.mat[1][6] = 1;
	expected_matrix.mat[1][7] = 1;
	expected_matrix.mat[1][8] = 1;

	raster::scanConvertLine(matrix1, Point(1,8), Point(1,1));
	raster::scanConvertLine(matrix2, Point(1,1), Point(1,8));

	ASSERT_EQUAL(matrix1.mat, expected_matrix.mat);
	ASSERT_EQUAL(matrix2.mat, expected_matrix.mat);
}
int main() 
{
	scan_convert_line_test1();
	scan_convert_line_test2();
	scan_convert_line_test3();
	scan_convert_line_test4();
}