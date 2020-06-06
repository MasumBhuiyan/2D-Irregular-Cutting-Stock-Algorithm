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

/** namespace linear */
/**
 * checks p is on the segment of a, b or not
*/
bool linear::isOnSegment(Point p, Point a, Point b)
{
    return (geo::dcmp(vec::cross(a - p, b - p)) == 0 and
            geo::dcmp(vec::dot(a - p, b - p)) <= 0);
}

/**  namespace polygonal */
/**
 * checks point is inside the polygon or not 
*/
bool polygonal::isPointInsidePolygon(Point point, Polygon &polygon)
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
    return windingNumber ? -1 : 1;
}