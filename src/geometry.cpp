#include "geometry.hpp"

int geo ::dcmp(double x, double eps) // double compare
{
    if (fabs(x) < eps)
        return 0; // for double

    return x < 0 ? -1 : 1;
}

double geo::DEG2RAD(double degree) { return (degree * (PI / 180.0)); }
double geo::RAD2DEG(double radian) { return (radian * (180.0 / PI)); }