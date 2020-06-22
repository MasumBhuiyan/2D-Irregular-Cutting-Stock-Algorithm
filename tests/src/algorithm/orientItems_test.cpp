#include "algorithm.hpp"
#include <fstream>
#include <Item.hpp>
#include <Matrix.hpp>
#include <Point.hpp>

void orientItems_test1()
{
	std::vector<Item> items;
	items.push_back(Item(4, vector<Point>({Point(0 * 2, 0 * 2),
										   Point(5 * 2, 0 * 2),
										   Point(5 * 2, 5 * 2),
										   Point(0 * 2, 5 * 2)})));
	items.push_back(Item(3, vector<Point>({Point(0 * 2, 0 * 2),
										   Point(5 * 2, 0 * 2),
										   Point(5 * 2, 5 * 2)})));
	items.push_back(Item(8, vector<Point>({Point(0 * 2, 1 * 2),
										   Point(2 * 2, 0 * 2),
										   Point(5 * 2, 0 * 2),
										   Point(7 * 2, 1 * 2),
										   Point(7 * 2, 4 * 2),
										   Point(5 * 2, 5 * 2),
										   Point(2 * 2, 5 * 2),
										   Point(0 * 2, 4 * 2)})));

	vector<Matrix> matrices;
	int idx = 1;
	for(auto i : items) 
	{
		matrices.push_back(i.rasterize(idx));
		idx += 1;
		matrices.back().print();
	}

	Matrix stock = orientItems(matrices);
}
int main()
{
	orientItems_test1();
	return 0;
}