#ifndef RASTERIZATION_HPP
#define RASTERIZATION_HPP

#include <algorithm>
#include <cassert>

#include "geometry.hpp"
#include "Matrix.hpp"

#define MAX_X 1e9
#define MAX_Y 1e9

namespace raster
{
    void translateToNewOrigin(Polygon &, double &, double &);
    std::pair<int, int> getRasterMatrixDimension(Polygon &);
    std::pair<int, int> getSeedPoint(Matrix &, Polygon &);
    void scanConvertLine(Matrix &, Point, Point);
    void regionfill(Matrix &, int, int);
}; // namespace raster

#endif // RASTERIZATION_HPP