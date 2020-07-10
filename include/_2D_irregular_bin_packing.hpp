#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <chrono>

#include <boost_geometry_utility.hpp>

namespace zero_waste_apparels
{
    void solution(std::vector<PolygonInput> &, double &);
    void placeItem(MultiPolygon &, Polygon &, double &);
    void normalize(PolygonInput &);
    void normalizePolygon(Polygon &);
    double getLength(Polygon &);
    double getWidth(Polygon &);
}; // namespace zero_waste_apparels

#endif // _2D_IRREGULAR_BIN_PACKING