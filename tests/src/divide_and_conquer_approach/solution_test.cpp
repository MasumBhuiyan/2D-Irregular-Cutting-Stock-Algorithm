#include "divide_and_conquer_approach.hpp"

void solution_test1()
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
    
    DnCApproach::solution(items);

}

int main()
{
	solution_test1();
	return 0;
}