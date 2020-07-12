#ifndef _2D_IRREGULAR_BIN_PACKING
#define _2D_IRREGULAR_BIN_PACKING

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <deque>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <chrono>

#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>
#include <boost/geometry/geometries/geometries.hpp>

namespace boost_geo = boost::geometry;
namespace trans = boost::geometry::strategy::transform;

typedef boost_geo::model::point<double, 2, boost_geo::cs::cartesian> Point;
typedef boost_geo::model::polygon<Point> Polygon;
typedef boost_geo::model::multi_polygon<Polygon> MultiPolygon;
typedef boost_geo::model::box<Point> Box;
typedef std::vector<std::pair<double, double>> PolygonInput;
typedef Point Vector;

#define x get<0>()
#define y get<1>()

const double EPS = 1e-8;
const double INF = 4e18;
const double PI = acos(-1);

static int frameno;

namespace boost_geo_util
{
    double getDirectedAngle_D(Vector, Vector);
    Polygon constructBGPolygon(PolygonInput &);
    MultiPolygon unionPolygons(MultiPolygon &, MultiPolygon &);
    MultiPolygon reflectAcrossLine(MultiPolygon &, Point, Point);
    bool isPolygonIntersectPolygon(MultiPolygon &, MultiPolygon &);
    bool isPointInsidePolygons(MultiPolygon &, Point &);
    void visualize(MultiPolygon &, std::string prfx = "");
}; // namespace boost_geo_util

namespace zero_waste_apparels
{
    void runDataset(std::string);
    std::vector<PolygonInput> readDataset(std::string, std::vector<int> &, double &);
    void solution(std::vector<PolygonInput> &, double &);
    void placeItem(MultiPolygon &, Polygon &, double &);
    void normalize(PolygonInput &);
    void normalizePolygon(Polygon &);
    double getLength(Polygon &);
    double getWidth(Polygon &);
}; // namespace zero_waste_apparels

#endif // _2D_IRREGULAR_BIN_PACKING
