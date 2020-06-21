#ifndef DIVIDE_AND_CONQUER_APPROACH_HPP
#define DIVIDE_AND_CONQUER_APPROACH_HPP

#include <vector>
#include <tuple>
#include <cassert>

#include "Point.hpp"
#include "Item.hpp"
#include "Matrix.hpp"
#include "geometry.hpp"

struct ItemState
{
    /* data */
    int itemId;
    double rotatedBy;
    Point pivot;

    ItemState() {}
    ItemState(int id, double angle, Point p) : itemId(id), rotatedBy(angle), pivot(p) {}
};

namespace DnCApproach
{
    const double INF = 1e18;
    const int RASTER_MATRIX_DIMENSION = 1000;
    static std::vector<double> rotations({45, 90, 135, 180, 225, 270, 315});

    Matrix rasterItems(
        std::vector<Item> &,
        std::vector<ItemState> &,
        int rasterDimension = RASTER_MATRIX_DIMENSION);
    std::pair<double, Point> mergeItemToFindMinEnclosingRectangleArea(Matrix &, Matrix &);
    std::pair<double, Point> mergeItemsSet(
        std::vector<Item> &,
        std::vector<ItemState> &,
        std::vector<ItemState> &);
    std::vector<ItemState> rotateItemStates(std::vector<ItemState> &, double);
    std::vector<ItemState> findBestOrientation(
        std::vector<Item> &,
        std::vector<ItemState> &,
        std::vector<ItemState> &);
    std::vector<ItemState> divideAndConquer(std::vector<Item> &, int, int);
    void solution(std::vector<Item> &);
    double packingDensity(Matrix &);
}; // namespace DnCApproach

#endif // DIVIDE_AND_CONQUER_APPROACH_HPP