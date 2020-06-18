#include "Stock.hpp"
#include "Point.hpp"
#include "custom_assertion.hpp"

void isPlaceable_test1()
{
	Stock stock(15,15);
	Matrix matrix(15,15);

	for(int i = 5; i <= 9; i += 1)
	{
		for(int j = 5; j <= 9; j += 1)
		{
			stock.mat[ i ][ j ] = 1;
			matrix.mat[ i ][ j ] = 1;
		}
	}

	stock.print();
	matrix.print();

	bool isPlaced = stock.isPlaceable(Point(0.0,0.0), matrix);
	ASSERT_EQUAL(isPlaced, false);

	isPlaced = stock.isPlaceable(Point(4.0,4.0), matrix);
	ASSERT_EQUAL(isPlaced, false);

	isPlaced = stock.isPlaceable(Point(5.0,5.0), matrix);
	ASSERT_EQUAL(isPlaced, true);
}

int main() 
{	
	isPlaceable_test1();
	return 0;
}