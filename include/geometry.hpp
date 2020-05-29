#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <tuple>
#include <vector>
#include <cassert>
#include <algorithm>

#include "polygon.hpp"

using namespace std;

namespace geo
{
tuple<int, int> minRectangleEnclosure(polygon);
tuple<int, int> orient(polygon &);
vector<polygon> sort(vector<polygon> &);
}; // namespace geo

#endif // GEOMETRY_HPP