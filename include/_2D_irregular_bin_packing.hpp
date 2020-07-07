#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <iostream>
#include <iomanip>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <algorithm>

#include <boost_geometry_utility.hpp>

namespace dnc_approach
{
    void mergeHeuristics1Placement(MultiPolygon &, MultiPolygon &,
                                   Point, Point, Point, Point,
                                   double &, MultiPolygon &);
    MultiPolygon mergeHeuristics1(MultiPolygon &, MultiPolygon &);
    MultiPolygon mergeMultiPolygons(MultiPolygon &, MultiPolygon &);
    MultiPolygon split(std::vector<Polygon> &, int, int);
    void solution(std::vector<PolygonInput> &items);

}; // namespace dnc_approach

#endif // _2D_IRREGULAR_BIN_PACKING