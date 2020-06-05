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
double vec::dot(Vector a, Vector b) { return a.x * b.x + a.y * b.y; }
double vec::cross(Vector a, Vector b) { return a.x * b.y - a.y * b.x; }
double vec::lenSq(Vector a) { return a.x * a.x + a.y * a.y; }
double vec::len(Vector a) { return sqrt(lenSq(a)); }
double vec::getAngleInRad(Vector a, Vector b) { return acos(dot(a, b) / (len(a) * len(b))); }