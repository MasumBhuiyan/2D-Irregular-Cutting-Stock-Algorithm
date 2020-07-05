#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <vector>
// #include <tuple>
#include <string>
#include <cstring>

#include <boost_geometry_utility.hpp>

// #include <boost/foreach.hpp>
// #include <boost/geometry.hpp>
// #include <boost/geometry/geometries/point_xy.hpp>
// #include <boost/geometry/geometries/polygon.hpp>

// typedef boost::geometry::model::d2::point_xy<double> point;
// typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

// const double EPS = 1e-8;
// const double INF = 4e18;
// const double PI = acos(-1);

// #define X first
// #define Y second
// typedef pair<double, double> Point;
// typedef vector<pair<double, double>> Polygon;
typedef std::pair<std::vector<Polygon>, Polygon> ItemState;

namespace dnc_approach
{
    // string toStringPoly(Polygon);
    // void printPolygon(Polygon);
    // Polygon polygonUnion(Polygon, Polygon);
    // double minBoundingRectangle(Polygon);
    // double areaPoly(Polygon);
    // Point rotatePoint(Point, Point, double);
    // Polygon rotatePolygon(Polygon, Point, double);
    // Polygon reflectAcrossLine(Polygon, Point, Point);
    // double findRotationAngle(Point, Point);
    // Polygon placement(Polygon, Point, Point, Point, Point);
    // double polygonPolygonIntersectionArea(Polygon, Polygon);
    // Polygon normalize(Polygon);
    // void readIO(string, vector<Polygon> &, vector<int> &, double &);
    // void processVisualize(vector<Polygon>);
    ItemState translateItemState(ItemState &, Point);
    ItemState rotateItemState(ItemState &, Point, double);
    ItemState mergeHeuristics1(ItemState &, ItemState &);
    // ItemState Normalize(ItemState);
    // bool doesOverlap(ItemState, ItemState);
    // ItemState merge(ItemState, ItemState);
    ItemState mergeItemStates(ItemState &, ItemState &);
    ItemState split(std::vector<Polygon> &, int, int);
    ItemState solution(std::vector<PolygonInput> &items);

}; // namespace dnc_approach

#endif // _2D_IRREGULAR_BIN_PACKING