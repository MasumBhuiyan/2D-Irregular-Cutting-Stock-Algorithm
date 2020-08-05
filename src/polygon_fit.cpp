#include <polygon_fit.hpp>

Polygon polygon_fit::getInnerFitRectangle(std::vector<Polygon> cluster, double length, double width)
{
    Polygon innerFitRectangle;
    cluster = geo_util::normalize(cluster);
    double min_x = INF, min_y = INF, max_x = -INF, max_y = -INF;
    for(auto polygon: cluster)
    {
        for(auto point: polygon.outer())
        {
            min_x = std::min(min_x, point.x);
            max_x = std::max(max_x, point.x);
            min_y = std::min(min_y, point.y);
            max_y = std::max(max_y, point.y);
        }
    }
    innerFitRectangle.outer().push_back(Point(std::abs(min_x), std::abs(min_y)));
    innerFitRectangle.outer().push_back(Point(std::abs(min_x), length - max_y));
    innerFitRectangle.outer().push_back(Point(width - max_x, length - max_y));
    innerFitRectangle.outer().push_back(Point(width - max_x, std::abs(min_y)));
    innerFitRectangle.outer().push_back(Point(std::abs(min_x), std::abs(min_y)));
    return innerFitRectangle;
}

Polygon polygon_fit::getNoFitPolygon(Polygon &polygon, std::vector<Polygon> cluster)
{
    Polygon noFitPolygon;
    return noFitPolygon;
}
std::vector<Polygon> polygon_fit::getAllNfpIfr(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> cluster)
{
    std::vector<Polygon> allNfpIfr;
    return allNfpIfr;
}
std::vector<Point> polygon_fit::getAllEdgeIntersectionPoints(std::vector<Polygon> &polygons)
{
    std::vector<Point> allEdgeIntersectionPoints;
    return allEdgeIntersectionPoints;
}