#include <_2D_irregular_bin_packing.hpp>

/**
 * first heuristics method for placement
*/
MultiPolygon dnc_approach::mergeHeuristics1(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{

    double minAreaOfMBR = INF; // MBR = Minimum Bound Rectangle of a ploygon
    int angleInterval = 90;
    for (int angle_i = angleInterval; angle_i < 360; angle_i += angleInterval)
    {
        MultiPolygon multiPolygon_a = boost_geo_util::rotate(multiPolygonA, Point(0, 0), angle_i);
        for (Polygon &polygonB_i : multiPolygonB)
        {
        }
    }
}

/**
 * merge 
 */
MultiPolygon dnc_approach::mergeMultiPolygons(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    if (boost_geo::area(multiPolygonA) < boost_geo::area(multiPolygonB))
    {
        std::swap(multiPolygonA, multiPolygonB);
    }
    return mergeHeuristics1(multiPolygonA, multiPolygonB);
}

/**
 * divide and conquer
 */
MultiPolygon dnc_approach::split(std::vector<Polygon> &polygons, int left, int right)
{
    if (left == right)
    {
        return MultiPolygon({polygons[left]});
    }
    int mid = left + (right - left) / 2;

    MultiPolygon multiPolygonLeft = split(polygons, left, mid);
    MultiPolygon multiPolygonRight = split(polygons, mid + 1, right);

    return mergeMultiPolygons(multiPolygonLeft, multiPolygonRight);
}

MultiPolygon dnc_approach::solution(std::vector<PolygonInput> &items)
{
    int numberOfItems = items.size();
    std::vector<Polygon> polygons(numberOfItems);
    for (int i = 0; i < numberOfItems; i++)
    {
        polygons[i] = boost_geo_util::constructBGPolygon(items[i]);
    }
    return split(polygons, 0, numberOfItems - 1);
}