#include "divide_and_conquer_approach.hpp"

void raster_items_test1()
{
	std::vector<Item> items;
	std::vector<ItemState> itemStates;

	items.push_back(Item(4, vector<Point>({Point(0, 0), Point(5, 0), Point(5, 5), Point(0, 5)})));
	items.push_back(Item(3, vector<Point>({Point(0, 0), Point(5, 0), Point(5, 5)})));
	items.push_back(Item(8, vector<Point>({Point(0, 1), Point(2, 0), Point(5, 0), Point(7, 1), Point(7, 4), Point(5, 5), Point(2, 5), Point(0, 4)})));

	itemStates.push_back(ItemState(0, 0, Point(0, 0)));
	itemStates.push_back(ItemState(1, 90, Point(5, 5)));
	itemStates.push_back(ItemState(2, 0, Point(5, 0)));

	Matrix stock = DnCApproach::rasterItems(items, itemStates, 50);
	stock.print();
}
int main()
{
	raster_items_test1();
	return 0;
}