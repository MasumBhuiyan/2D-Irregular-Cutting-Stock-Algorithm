#ifndef POLYGON_FIT
#define POLYGON_FIT

#include <geo_util.hpp>

namespace polygon_fit 
{
    Polygon innerFitPolygon(Polygon&, double, double);
    Polygon noFitPolygon(Polygon&, Polygon&);
    void allInnerFitPolygons(std::vector<Polygon>&, double, double);
    void allPairNoFitPolygons(std::vector<Polygon>&);
};

#endif