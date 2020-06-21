#ifndef DIVIVE_N_CONQUER_APPROACH_HPP
#define DIVIVE_N_CONQUER_APPROACH_HPP

#include <vector>
#include <tuple>
#include <cassert>

#include "Point.hpp"
#include "Item.hpp"
#include "Matrix.hpp"
#include "geometry.hpp"
#include "rasterization.hpp"

static std::vector<double> rotations({45, 90, 135, 180, 225, 270, 315});

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
}; // namespace DnCApproach

#endif // DIVIVE_N_CONQUER_APPROACH_HPP