#ifndef BOOST_GEOMETRY_UTILITY_HPP
#define BOOST_GEOMETRY_UTILITY_HPP

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include <boost/geometry.hpp>
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
    void visualize(MultiPolygon &);
}; // namespace boost_geo_util

#endif // BOOST_GEOMETRY_UTILITY_HPP