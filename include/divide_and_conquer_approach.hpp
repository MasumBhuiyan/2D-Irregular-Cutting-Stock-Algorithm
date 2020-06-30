#ifndef DIVIVE_N_CONQUER_APPROACH_HPP
#define DIVIVE_N_CONQUER_APPROACH_HPP

#include <Item.hpp>
#include <Point.hpp>
#include <geometry.hpp>

#define INF 4e18

typedef std::pair<Item, double> ItemState;

namespace approach1
{
	int orient(Point, Point, Point);
	std::pair<Point,Point> findLargestEdge(Item &);
	double findRotationAngle(Point, Point);
	Item placement(Item, Point, Point, Point, Point);
	Item outerface(Item, Item, Point, Point, Point, Point);
    Item mergeHeuristic1(Item &, Item &);
    std::pair<double, double> minAreaRectangle(Item &);
    ItemState mergeItem(ItemState &, ItemState &);
    ItemState split(std::vector<Item> &, int, int);
    void solution(std::vector<Item> &);
}; // namespace approach1

#endif // DIVIVE_N_CONQUER_APPROACH_HPP