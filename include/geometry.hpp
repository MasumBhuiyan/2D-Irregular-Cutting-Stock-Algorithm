#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <functional>

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
    double dot(Vector, Vector);                    // dot product
    double cross(Vector, Vector);                  // cross product
    double lenSq(Vector);                          // lenth square
    double len(Vector);                            // length
    double getAngleInRad(Vector, Vector);          // angle inside two vector
    bool pointInRectangle(Vector, Vector, Vector); // is point inside rectangle
    int orient(Vector, Vector, Vector c);
    bool segSegIntersection(Vector, Vector, Vector, Vector); // segment segment intersection

}; // namespace vec

namespace linear
{
    bool isOnSegment(Point, Point, Point);              // returns true if the 1st Point is on the segment of 2nd Point & 3rd Point
    int segCrossSeg(Point, Point, Point, Point);        // segment segment relation
    bool segCrossSegInside(Point, Point, Point, Point); // segment cross segment strickly inside

}; // namespace linear

typedef std::vector<Point> Polygon;
namespace polygonal
{
    int isPointInsidePolygon(Point, Polygon &);         // checks if the Point is inside the Polygon
    Polygon changeOrientation(Polygon &);               // converts cw if the the polygon is ccw vice-versa
    Polygon translateToNewOrign(Point, Polygon &);      // translates to the point
    Polygon rotateBy(double, Point, Polygon &);         // rotate by angle with respect to point
    bool polygonIntersectPolygon(Polygon &, Polygon &); // polygon polygon relation
    void printPolygon(Polygon &);                       // prints on console

}; // namespace polygonal

namespace no_fit_polygon
{
    Polygon nfpConvex(Polygon &, Polygon &);         // no fit polygon of two convex polygons
    Polygon nfpSimplePolygons(Polygon &, Polygon &); // no fit polygon of two simple polygons polygons
};                                                   // namespace no_fit_polygon
#endif                                               // GEOMETRY_HPP