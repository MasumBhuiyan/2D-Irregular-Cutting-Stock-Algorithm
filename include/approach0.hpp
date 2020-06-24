#ifndef APPROACH0_HPP
#define APPROACH0_HPP


#include <Point.hpp>
#include <Item.hpp>
#include <set>
#include <geometry.hpp>

namespace approach0 
{
	double minRecArea(std::vector<Item>&);
	void rotate(std::vector<Item> &, double);
	bool doesOverlap(std::vector<Item>&, std::vector<Item>&);
	std::tuple<double, std::vector<Item>> merge(std::vector<Item> &, std::vector<Item> &);
	std::vector<Item> split(std::vector<Item> &, int, int);
	void solution(std::vector<Item>&);
};
#endif // APPROACH0_HPP