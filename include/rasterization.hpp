#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <algorithm>

#include "geometry.hpp"
#include "Matrix.hpp"

#define MAX_X 1e9
#define MAX_Y 1e9

namespace raster
{
    void translateToNewOrigin(Polygon &, double &, double &, Point &);
    std::pair<int, int> getRasterMatrixDimension(Polygon &, Point &);
    void scanConvertLine(Matrix &, Point, Point);
    void regionfill(Matrix &, int, int);
}; // namespace raster

#endif // RASTERIZATION_HPP