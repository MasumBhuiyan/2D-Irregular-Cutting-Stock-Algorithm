#ifndef BIN_PACKING
#define BIN_PACKING

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

/**
 * customized geometry utility methods using boost
 * */
namespace boost_geo_util 
{
    Polygon constructBGPolygon(PolygonInput &);
    bool isPolygonIntersectPolygon(MultiPolygon &, MultiPolygon &);
    bool isPointInsidePolygons(MultiPolygon &, Point &);
    void visualize(MultiPolygon &, std::string);
}; // namespace boost_geo_util

namespace bin_packing
{
    void runDataset(std::string, std::string, double);
    std::vector<PolygonInput> readDataset(std::string, std::vector<int> &, double &);
    void binPacking(std::vector<PolygonInput> &, double, double &, std::string);
    void placeItem(MultiPolygon &, Polygon &, double, double &);
    void normalize(PolygonInput &);
    void normalizePolygon(Polygon &);
    double getLength(Polygon &);
    double getWidth(Polygon &);
}; // namespace bin_packing

#endif // BIN_PACKING
