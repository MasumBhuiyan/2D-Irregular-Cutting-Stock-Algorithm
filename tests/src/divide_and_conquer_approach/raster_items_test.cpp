#include <iostream>

#include "divide_and_conquer_approach.hpp"

void raster_items_test1()
{
	std::vector<Item> items;
	std::vector<ItemState> itemStates;

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

	itemStates.push_back(ItemState(0, 51, Point(0, 0)));
	itemStates.push_back(ItemState(1, 90, Point(35, 0)));
	itemStates.push_back(ItemState(2, 39, Point(20, 26)));

	for (int i = 0; i < 3; i++)
	{
		Item item = items[i];
		std::cout << "Item: " << std::endl;
		item.print();
		std::cout << "Item raster: " << std::endl;
		item.rasterize().print();
		std::cout << "After rotation: " << std::endl;
		Item tmpItem = item.rotate(itemStates[i].rotatedBy, {0, 0});
		tmpItem.rasterize().print();
	}

	Matrix stock = DnCApproach::rasterItems(items, itemStates, 90);
	stock.print();
}
int main()
{
	raster_items_test1();
	return 0;
}