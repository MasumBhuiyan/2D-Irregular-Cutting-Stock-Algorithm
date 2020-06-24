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
// pointInRectangle(point a,point b,point c): true if c is strictly inside or 
// in the boundary of the rectangle bounded by a and b
bool vec::pointInRectangle(Vector a, Vector b, Vector c) {//
    bool p = ( std::min(a.x, b.x) <= c.x && std::max(a.x, b.x) >= c.x );
    bool q = ( std::min(a.y, b.y) <= c.y && std::max(a.y, b.y) >= c.y );
    return (p && q);
}

// orient(point a, point b, point c): 0 if c is on AB, 1 if left, -1 if right
int vec::orient(Vector a, Vector b, Vector c) {//
    double p = vec::cross(b - a, c - a);
    if( fabs(p) < geo::EPS ) return 0;
    return p > 0 ? 1 : -1;
}

// segSegIntersection(point a, point b, point c, point d): true if AB intersects CD
bool vec::segSegIntersection(Vector a, Vector b, Vector c, Vector d) { //
    int o1 = vec::orient(a, b, c), o2 = vec::orient(a, b, d);
    int o3 = vec::orient(c, d, a), o4 = vec::orient(c, d, b);
    
    if( o1 * o2 == -1 && o3 * o4 == -1 ) return true;
    if( (!o1 && vec::pointInRectangle(a, b, c)) || (!o2 && vec::pointInRectangle(a, b, d)) ) return true; // touching not allowed
    if( (!o3 && vec::pointInRectangle(c, d, a)) || (!o4 && vec::pointInRectangle(c, d, b)) ) return true;
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
            return 1;
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
    return windingNumber ? 1 : 0; // touch not allowed
}