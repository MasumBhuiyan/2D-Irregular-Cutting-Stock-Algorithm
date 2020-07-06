#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <cstring>

#include <boost_geometry_utility.hpp>

namespace dnc_approach
{
    MultiPolygon mergeHeuristics1(MultiPolygon &, MultiPolygon &);
    MultiPolygon mergeMultiPolygons(MultiPolygon &, MultiPolygon &);
    MultiPolygon split(std::vector<Polygon> &, int, int);
    MultiPolygon solution(std::vector<PolygonInput> &items);

}; // namespace dnc_approach

#endif // _2D_IRREGULAR_BIN_PACKING