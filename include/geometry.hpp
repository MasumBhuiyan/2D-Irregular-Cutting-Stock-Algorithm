#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
#include <vector>

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
    double dot(Vector, Vector);           // dot product
    double cross(Vector, Vector);         // cross product
    double lenSq(Vector);                 // lenth square
    double len(Vector);                   // length
    double getAngleInRad(Vector, Vector); // angle inside two vector

}; // namespace vec

namespace linear
{
    bool isOnSegment(Point, Point, Point); // returns true if the 1st Point is on the segment of 2nd Point & 3rd Point

}; // namespace linear

typedef std::vector<Point> Polygon;
namespace polygonal
{
    int isPointInsidePolygon(Point, Polygon &); // checks if the Point is inside the Polygon

}; // namespace polygonal

#endif // GEOMETRY_HPP