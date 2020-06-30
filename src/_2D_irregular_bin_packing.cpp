#include <_2D_irregular_bin_packing.hpp>

Polygon dnc_approach::mergePolygons(Polygon &polygon1, Polygon &polygon2)
{
    if (polygon1.area() < polygon2.area())
    {
        std::swap(polygon1, polygon2);
    }

    Polygon mergedPolygon;
    double angle = 90;
    double minArea = INF, length = INF;

    for (int i = 0; i < (360 / (int)angle); i++)
    {
        // Item tmpMergedItem = mergeHeuristic1(item1, item2);
        // auto minRectangle = minAreaRectangle(tmpMergedItem);
        // double tmpArea = minRectangle.first * minRectangle.second;
        // if (tmpArea < minArea)
        // {
        //     mergedItem = tmpMergedItem;
        //     minArea = tmpArea;
        //     length = minRectangle.first;
        // }
        // else if (tmpArea == minArea and length > minRectangle.first)
        // {
        //     mergedItem = tmpMergedItem;
        //     length = minRectangle.first;
        // }
        // item1 = item1.rotate(geo::DEG2RAD(angle), Point(0, 0));
        }

    return mergedPolygon;
}

Polygon dnc_approach::split(std::vector<Polygon> &items, int left, int right)
{
    if (left == right)
    {
        return items[left];
    }
    int mid = left + (right - left) / 2;
    Polygon polygonLeft = split(items, left, mid);
    Polygon polygonRight = split(items, mid + 1, right);

    return mergePolygons(polygonLeft, polygonRight);
}

void dnc_approach::solution(std::vector<Polygon> &items)
{
    std::cout << "Number of Item: " << items.size() << std::endl;
    for (Polygon item : items)
    {
        std::cout << item << std::endl;
    }
    // Polygon mergedItems = split(items, 0, items.size() - 1);
}