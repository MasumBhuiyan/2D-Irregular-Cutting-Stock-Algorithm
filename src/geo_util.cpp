#include <geo_util.hpp>

/**
* compares two double variables a, b
* d = a - b
* returns 0 if a == b, 1 if a > b, -1 if a < b
*/
int geo_util::dblcmp(double d, double eps)
{
    if (std::fabs(d) < eps)
	{
		return 0;
	}
	return d > eps ? 1 : -1;
}

/**
* checks concavity of a polygon
* returns true if polygon is concave, false otherwise
*/
bool geo_util::isConcave(Polygon &polygon)
{
    int n = polygon.outer().size();
    if ( n < 4 ) return true;

    bool sign = false;
    for(int i = 0; i < n; i += 1)
    {
        double dx1 = polygon.outer()[(i + 2) % n].x - polygon.outer()[(i + 1) % n].x;
        double dy1 = polygon.outer()[(i + 2) % n].y - polygon.outer()[(i + 1) % n].y;
        double dx2 = polygon.outer()[i].x - polygon.outer()[(i + 1) % n].x;
        double dy2 = polygon.outer()[i].y - polygon.outer()[(i + 1) % n].y;
        double zcrossproduct = dx1 * dy2 - dy1 * dx2;
        if (i == 0) sign = zcrossproduct > 0;
        else if (sign != (zcrossproduct > 0)) return true;
    }

    return false;
}

/**
* returns length of a polygon
*/
double geo_util::getLength(Polygon &polygon)
{
    double min_x = INF, max_x = -INF;
    for(auto point: polygon.outer())
    {
        min_x = std::min(point.x, min_x);
        max_x = std::max(point.x, max_x);
    }
    return max_x - min_x;
}

/**
* returns width of a polygon
*/
double geo_util::getWidth(Polygon &polygon)
{
    double min_y = INF, max_y = -INF;
    for(auto point: polygon.outer())
    {
        min_y = std::min(point.y, min_y);
        max_y = std::max(point.y, max_y);
    }
    return max_y - min_y;
}

/**
* cross product of vector OP and OQ
*/ 
double geo_util::crossProduct(Point p, Point q)
{
	return p.x * q.y - p.y * q.x;
}

/**
* translates coordinates into local coordinates
*/
void geo_util::normalize(Polygon &polygon)
{
    double min_x = INF;
    double min_y = INF;
    for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
    {
        min_x = std::min(min_x, (*it).x);
        min_y = std::min(min_y, (*it).y);
    }
    for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
    {
        Point t = *it;
        t = Point(t.x - min_x, t.y - min_y);
        *it = t;
    }
}
/**
* determines the side of point c about the line ab
* returns 0 if c is on the line of ab
*         1 if c is on the positive side of ab
*        -1 if c is on the negative side of ab
*/
int geo_util::orientation(Point a, Point b, Point c)
{
    double x1 = b.x, y1 = b.y;
	double x2 = c.x, y2 = c.y;
	double _x = a.x, _y = a.y;
	double p = crossProduct(Point(x1 - _x, y1 - _y), Point(x2 - _x, y2 - _y));
	if (p == 0) return 0;
	return p < 0 ? -1 : 1;
}

/**
* rotate polygon counter clockwise at a degree(0 - 360)
* w.r.t a reference point
*/
Polygon geo_util::rotatePolygon(Polygon &polygon, Point reference, double degree)
{
    Polygon rotatedPolygon;
    boost_geo::transform(polygon, rotatedPolygon, trans::rotate_transformer<boost_geo::degree, double, 2, 2>(degree));
    return rotatedPolygon;
}

/**
* translate a polygon to a point
*/
Polygon geo_util::translatePolygon(Polygon &polygon, Point point)
{
    Polygon translatedPolygon;
    boost_geo::transform(polygon, translatedPolygon, trans::translate_transformer<double, 2, 2>(point.x, point.y));
    return translatedPolygon;
}
/**
* perpendicular distance of a point p to a line ab
*/
double geo_util::linePointDistance(Point a, Point b, Point p)
{
    Line line;
    line.push_back(a);
    line.push_back(b);
    return boost_geo::distance(p, line);
}
/**
* returns the intersection area of two polygons
*/
double geo_util::polygonPolygonIntersectionArea(Polygon &polygon1, Polygon &polygon2)
{
    std::deque<Polygon> polygons;
    boost_geo::intersection(polygon1, polygon2, polygons);
    double intersectedArea = 0.0;
    for(auto polygon: polygons)
    {
        intersectedArea += boost_geo::area(polygon);
    }
    return intersectedArea;
}
/**
*   returns (INF, INF) is ab does not intersect cd
*           intersection point otherwise
*/
Point  geo_util::segmentSegmentIntersectionPoint(Point a, Point b, Point c, Point d) { //
    int o1 = geo_util::orientation(a, b, c), o2 = geo_util::orientation(a, b, d);
    int o3 = geo_util::orientation(c, d, a), o4 = geo_util::orientation(c, d, b);

    if((o1 * o2 == -1 && o3 * o4 == -1) 
       || (!o1 && geo_util::pointInRectangle(a, b, c)) || (!o2 && geo_util::pointInRectangle(a, b, d))
       || (!o3 && geo_util::pointInRectangle(c, d, a)) || (!o4 && geo_util::pointInRectangle(c, d, b))
    ) 
    {
        double a1 = geo_util::crossProduct(Point(d.x - c.x, d.y - c.y), Point(a.x - c.x, a.y - c.y));
		double a2 = geo_util::crossProduct(Point(d.x - c.x, d.y - c.y), Point(b.x - d.x, b.y - d.y));
		return Point((a.x * a2 - b.x * a1) / (a2 - a1), (a.y * a2 - b.y * a1) / (a2 - a1));
    }
    return Point(INF, INF);
}
/**
* rotates a list of polygons at particular degree(0-360) 
* counter clockwise about a reference point
*/
std::vector<Polygon> geo_util::rotatePolygons(std::vector<Polygon> &polygons, Point reference, double degree)
{
    std::vector<Polygon> rotatedPolygons;
    for(auto &polygon: polygons)
    {
        rotatedPolygons.push_back(geo_util::rotatePolygon(polygon, reference, degree));
    }
    return rotatedPolygons;
}
/**
* translates a list of polygons to a point
*/
std::vector<Polygon> geo_util::translatePolygons(std::vector<Polygon> &polygons, Point point)
{
    std::vector<Polygon> translatedPolygons;
    for(auto &polygon: polygons)
    {
        translatedPolygons.push_back(geo_util::translatePolygon(polygon, point));
    }
    return translatedPolygons;
}
/**
* checks if it is possible to place a polygon
* in the container at a particular point
*/
bool geo_util::isItPossibleToPlacePolygon(std::vector<Polygon> &alreadyPlacedPolygons, Polygon polygonNextToBePlaced, Point point)
{
    polygonNextToBePlaced = geo_util::translatePolygon(polygonNextToBePlaced, point);
    for(auto poly: alreadyPlacedPolygons)
    {
        if( geo_util::polygonPolygonIntersectionArea(polygonNextToBePlaced, poly) > 0 ) return false;
    }
    return true;
}
/**
* checks whether a point c is in a rectangle
* enclosed by two points a, b
*/
bool geo_util::pointInRectangle(Point a, Point b, Point c) 
{
    bool p = ( std::min(a.x, b.x) <= c.x && std::max(a.x, b.x) >= c.x );
    bool q = ( std::min(a.y, b.y) <= c.y && std::max(a.y, b.y) >= c.y );
    return (p && q);
}
