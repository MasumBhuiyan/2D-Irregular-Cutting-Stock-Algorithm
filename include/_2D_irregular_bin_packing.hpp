#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

#define INF 4e18

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon;

namespace dnc_approach
{
    Polygon mergePolygons(Polygon &, Polygon &);
    Polygon split(std::vector<Polygon> &, int, int);
    void solution(std::vector<Polygon> &);
}; // namespace dnc_approach

#endif // _2D_IRREGULAR_BIN_PACKING