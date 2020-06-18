#include "Matrix.hpp"
#include "Point.hpp"
#include "custom_assertion.hpp"

void isIntersecting_test1()
{
	Matrix matrix1(15,15), matrix2(15,15);

	for(int i = 5; i <= 9; i += 1)
	{
		for(int j = 5; j <= 9; j += 1)
		{
			matrix1.mat[ i ][ j ] = 1;
			matrix2.mat[ i ][ j ] = 1;
		}
	}

	matrix1.print();
	matrix2.print();

	bool isIntersected = matrix1.isIntersecting(Point(0.0,0.0), matrix2);
	ASSERT_EQUAL(isIntersected, true);

	isIntersected = matrix1.isIntersecting(Point(4.0,4.0), matrix2);
	ASSERT_EQUAL(isIntersected, true);

	isIntersected = matrix1.isIntersecting(Point(5.0,5.0), matrix2);
	ASSERT_EQUAL(isIntersected, false);
}

int main() 
{	
	isIntersecting_test1();
	return 0;
}