#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>

#include "Point.hpp"

namespace geo
{
const double PI = acos(-1);
const double EPS = 1e-8;

int dcmp(double, double eps = EPS); // double compare
double DEG2RAD(double);             // degree to radian
double RAD2DEG(double);             // radian to degree

}; // namespace geo

typedef Point Vector;
namespace vec
{
double dot(Vector a, Vector b);
double cross(Vector a, Vector b);
double lenSq(Vector a);
double len(Vector a);
double getAngleInRad(Vector a, Vector b);
}; // namespace vec

#endif // GEOMETRY_HPP