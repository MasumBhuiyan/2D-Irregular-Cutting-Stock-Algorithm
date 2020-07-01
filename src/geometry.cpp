#include "geometry.hpp"

/** namespace geo */
int geo::dcmp(double x, double eps) // double compare
{
    if (fabs(x) < eps)
        return 0; // for double

    return x < 0 ? -1 : 1;
}

double geo::DEG2RAD(double degree) { return (degree * (PI / 180.0)); }
double geo::RAD2DEG(double radian) { return (radian * (180.0 / PI)); }

/** namespace vec */
/**
 *  dot product of a & b 
*/
double vec::dot(Vector a, Vector b) { return a.x * b.x + a.y * b.y; }
/** 
 * cross product of a & b 
 */
double vec::cross(Vector a, Vector b) { return a.x * b.y - a.y * b.x; }
/** 
 * (length)^2 and length of vector
*/
double vec::lenSq(Vector a) { return a.x * a.x + a.y * a.y; }
double vec::len(Vector a) { return sqrt(lenSq(a)); }
/**
 * angle between 2 vector
 * 0 <= angle <= PI/2
 */
double vec::getAngleInRad(Vector a, Vector b)
{
    return acos(dot(a, b) / (len(a) * len(b)));
}

/**
 *  pointInRectangle(point a,point b,point c): true if c is strictly inside or
 * in the boundary of the rectangle bounded by a and b
*/
bool vec::pointInRectangle(Vector a, Vector b, Vector c)
{
    bool p = (std::min(a.x, b.x) <= c.x and std::max(a.x, b.x) >= c.x);
    bool q = (std::min(a.y, b.y) <= c.y and std::max(a.y, b.y) >= c.y);
    return (p and q);
}

/**
 *  orient(point a, point b, point c): 0 if c is on AB, 1 if left, -1 if right
*/
int vec::orient(Vector a, Vector b, Vector c)
{ //
    double p = vec::cross(b - a, c - a);
    if (fabs(p) < geo::EPS)
        return 0;
    return p > 0 ? 1 : -1;
}

/**
 *  segSegIntersection(point a, point b, point c, point d): true if AB intersects CD
*/
bool vec::segSegIntersection(Vector a, Vector b, Vector c, Vector d)
{
    int o1 = vec::orient(a, b, c), o2 = vec::orient(a, b, d);
    int o3 = vec::orient(c, d, a), o4 = vec::orient(c, d, b);

    if (o1 * o2 == -1 and o3 * o4 == -1)
        return true;
    if ((!o1 and vec::pointInRectangle(a, b, c)) || (!o2 and vec::pointInRectangle(a, b, d)))
        return true; // touching not allowed
    if ((!o3 and vec::pointInRectangle(c, d, a)) || (!o4 and vec::pointInRectangle(c, d, b)))
        return true;
    return false;
}

/** namespace linear */
/**
 * checks p is on the segment of a, b or not
*/
bool linear::isOnSegment(Point p, Point a, Point b)
{
    return (geo::dcmp(vec::cross(a - p, b - p)) == 0 and
            geo::dcmp(vec::dot(a - p, b - p)) <= 0);
}

/**
 * returns 0 if does not intersect
 * returns 1 if non-standard intersection
 * returns 2 if intersects
*/
int linear::segCrossSeg(Point a, Point b, Point c, Point d)
{
    int d1 = geo::dcmp(vec::cross(b - a, c - a));
    int d2 = geo::dcmp(vec::cross(b - a, d - a));
    int d3 = geo::dcmp(vec::cross(d - c, a - c));
    int d4 = geo::dcmp(vec::cross(d - c, b - c));
    if ((d1 ^ d2) == -2 and (d3 ^ d4) == -2)
    {
        return 2;
    }
    return (d1 == 0 and geo::dcmp(vec::dot(c - a, c - b)) <= 0 or
            d2 == 0 and geo::dcmp(vec::dot(d - a, d - b)) <= 0 or
            d3 == 0 and geo::dcmp(vec::dot(a - c, a - d)) <= 0 or
            d4 == 0 and geo::dcmp(vec::dot(b - c, b - d)) <= 0);
}

/**
 * returns 1 if intersects strictly inside
 * returns 0 if not
*/
bool linear::segCrossSegInside(Point a, Point b, Point c, Point d)
{
    if (linear::isOnSegment(a, c, d) or linear::isOnSegment(b, c, d) or
        linear::isOnSegment(c, a, b) or linear::isOnSegment(d, a, b))
    {
        return false;
    }
    int d1 = geo::dcmp(vec::cross(b - a, c - a));
    int d2 = geo::dcmp(vec::cross(b - a, d - a));
    int d3 = geo::dcmp(vec::cross(d - c, a - c));
    int d4 = geo::dcmp(vec::cross(d - c, b - c));
    if ((d1 ^ d2) == -2 and (d3 ^ d4) == -2)
    {
        return true;
    }
    return (d1 == 0 and geo::dcmp(vec::dot(c - a, c - b)) <= 0 or
            d2 == 0 and geo::dcmp(vec::dot(d - a, d - b)) <= 0 or
            d3 == 0 and geo::dcmp(vec::dot(a - c, a - d)) <= 0 or
            d4 == 0 and geo::dcmp(vec::dot(b - c, b - d)) <= 0);
}

/**  namespace polygonal */
/**
 * checks point is inside the polygon or not 
 *  0  : if the point is on the borber
 *  1  : strictly inside the polygon
 * -1  : outside of the the polygon
*/
int polygonal::isPointInsidePolygon(Point point, Polygon &polygon)
{
    int n = polygon.size();
    int windingNumber = 0;
    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;
        if (linear::isOnSegment(point, polygon[i], polygon[j]) or point == polygon[i])
        {
            return 0;
        }
        int k = geo::dcmp(vec::cross(polygon[j] - polygon[i], point - polygon[i]));
        int d1 = geo::dcmp(polygon[i].y - point.y);
        int d2 = geo::dcmp(polygon[j].y - point.y);
        if (k > 0 and d1 <= 0 and d2 > 0)
        {
            windingNumber++;
        }
        if (k < 0 and d2 <= 0 and d1 > 0)
        {
            windingNumber--;
        }
    }
    return windingNumber ? 1 : -1;
}

/**
 * change orientation ccw to cw and cw to ccw
*/
Polygon polygonal::changeOrientation(Polygon &polygon)
{
    Polygon changedPolygon({polygon[0]});
    for (int i = polygon.size() - 1; i > 0; i--)
    {
        changedPolygon.push_back(polygon[i]);
    }
    return changedPolygon;
}

/**
 * translates to the point
*/
Polygon polygonal::translateToNewOrign(Point newOrigin, Polygon &polygon)
{
    Polygon translatedPolygon = polygon;
    for (Point &point : translatedPolygon)
    {
        point = point + newOrigin;
    }
    return translatedPolygon;
}

/**
 * rotate by angle with respect to point
 * angle is radian
*/
Polygon polygonal::rotateBy(double angle, Point origin, Polygon &polygon)
{
    Polygon rotatedPolygon = polygon;
    for (Point &point : rotatedPolygon)
    {
        point = point.rotate(origin, angle);
    }
    return rotatedPolygon;
}

/**
 * checks polygon1 intersect with polygon2 or not 
 *  0  : if non intersecting
 *  1  : if intersecting
 * polygons are simple polygon
*/
bool polygonal::polygonIntersectPolygon(Polygon &polygon1, Polygon &polygon2)
{
    int n = polygon1.size();
    int m = polygon2.size();
    assert(n >= 3 and m >= 3);
    // edge edge intersection
    for (int i = 0; i < n; i++)
    {
        Point p1 = polygon1[i];
        Point q1 = polygon1[(i + 1) % n];
        for (int j = 0; j < m; j++)
        {
            Point p2 = polygon2[j];
            Point q2 = polygon2[(j + 1) % m];
            if (linear::segCrossSegInside(p1, q1, p2, q2))
            {
                return true;
            }
        }
    }
    // check each point of two polygons
    for (Point point : polygon1)
    {
        if (polygonal::isPointInsidePolygon(point, polygon2) == 1)
        {
            return true;
        }
    }
    for (Point point : polygon2)
    {
        if (polygonal::isPointInsidePolygon(point, polygon1) == 1)
        {
            return true;
        }
    }
    return false;
}

/**
 * prints polygon on console
*/
void polygonal::printPolygon(Polygon &polygon)
{
    std::cout << "[";
    int n = polygon.size();
    for (int i = 0; i < n; i++)
    {
        std::cout << polygon[i].x << ", " << polygon[i].y;
        if (i != n - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

/** namespace no_fit_polygon */
/**
 * returns the no fit polygon of two convex polygons
*/
Polygon no_fit_polygon::nfpConvex(Polygon &a, Polygon &b)
{
}