#ifndef APPROACH0_HPP
#define APPROACH0_HPP


#include <Point.hpp>
#include <Item.hpp>
#include <set>
#include <geometry.hpp>

namespace approach0 
{
	std::tuple<double, vector<Item>> merge(std::vector<Item> &, std::vector<Item> &);
	void rotate(std::vector<Item> &, double);
	std::vector<Item> split(std::vector<Item> &, int, int);
};
#endif // APPROACH0_HPP