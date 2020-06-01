#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>

using namespace std;

namespace geo
{
const double PI = acos(-1);
const double EPS = 1e-8;

int dcmp(double, double eps = EPS); // double compare
double DEG2RAD(double);             // degree to radian
double RAD2DEG(double);             // radian to degree

}; // namespace geo

#endif // GEOMETRY_HPP