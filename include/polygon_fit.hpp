#ifndef POLYGON_FIT
#define POLYGON_FIT

#include <geo_util.hpp>

namespace polygon_fit 
{
    Polygon getInnerFitRectangle(std::vector<Polygon>, double, double);
    Polygon getNoFitPolygon(Polygon&, std::vector<Polygon>);
    std::vector<Polygon> getAllNfpIfr(std::vector<Polygon>&, std::vector<Polygon>);
    std::vector<Point> getAllEdgeIntersectionPoints(std::vector<Polygon>&);
};

#endif