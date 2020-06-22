#include "algorithm.hpp"
#include <fstream>
#include <Item.hpp>
#include <Matrix.hpp>
#include <Point.hpp>

void orientItems_test1()
{
	std::vector<Item> items;
	items.push_back(Item(4, vector<Point>({Point(0 * 5, 0 * 5),
										   Point(5 * 5, 0 * 5),
										   Point(5 * 5, 5 * 5),
										   Point(0 * 5, 5 * 5)})));
	items.push_back(Item(3, vector<Point>({Point(0 * 5, 0 * 5),
										   Point(5 * 5, 0 * 5),
										   Point(5 * 5, 5 * 5)})));
	items.push_back(Item(8, vector<Point>({Point(0 * 5, 1 * 5),
										   Point(2 * 5, 0 * 5),
										   Point(5 * 5, 0 * 5),
										   Point(7 * 5, 1 * 5),
										   Point(7 * 5, 4 * 5),
										   Point(5 * 5, 5 * 5),
										   Point(2 * 5, 5 * 5),
										   Point(0 * 5, 4 * 5)})));

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