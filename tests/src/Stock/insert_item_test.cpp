#include "Stock.hpp"
#include "Point.hpp"
#include "custom_assertion.hpp"

void insert_item_test1()
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

	cout << "Stock:\n";
	stock.print();
	cout << "Item to be inserted:\n";
	matrix.print();

	stock.insertItem(Point(5.0,5.0), matrix);
	cout << "Stock with inserted item:\n";
	stock.print();
}

int main() 
{	
	insert_item_test1();
	return 0;
}