#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <vector>
#include <tuple>
#include <string>
#include <cstring>

using namespace std;

#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

typedef boost::geometry::model::d2::point_xy<double> point;
typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> polygon;

const double EPS = 1e-8;
const double INF = 4e18;
const double PI = acos(-1);

#define X first
#define Y second
typedef pair<double, double> Point;
typedef vector<pair<double, double>> Polygon;

namespace dnc_approach
{
    string toStringPoly(Polygon);
    void printPolygon(Polygon);
    Polygon polygonUnion(Polygon, Polygon);
    double minBoundingRectangle(Polygon);
    double areaPoly(Polygon);
    Point rotatePoint(Point, Point, double);
    Polygon rotatePolygon(Polygon, Point, double);
    Polygon reflectAcrossLine(Polygon, Point, Point);
    double findRotationAngle(Point, Point);
    Polygon placement(Polygon, Point, Point, Point, Point);
    double polygonPolygonIntersectionArea(Polygon, Polygon);
    Polygon normalize(Polygon);
    void readIO(string, vector<Polygon> &, vector<int> &, double &);
    void processVisualize(vector<Polygon>);
    tuple<vector<Polygon>, Polygon> Translate(tuple<vector<Polygon>, Polygon>, Point);
    tuple<vector<Polygon>, Polygon> Rotate(tuple<vector<Polygon>, Polygon>, Point, double);
    tuple<vector<Polygon>, Polygon> Normalize(tuple<vector<Polygon>, Polygon>);
    bool doesOverlap(tuple<vector<Polygon>, Polygon>, tuple<vector<Polygon>, Polygon>);
    tuple<vector<Polygon>, Polygon> merge(tuple<vector<Polygon>, Polygon>, tuple<vector<Polygon>, Polygon>);
    tuple<vector<Polygon>, Polygon> split(vector<Polygon> &, int, int);

}; // namespace dnc_approach

#endif // _2D_IRREGULAR_BIN_PACKING