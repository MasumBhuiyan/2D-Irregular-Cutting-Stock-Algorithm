#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <deque>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/polygon/voronoi.hpp>
#include <boost/polygon/point_data.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>
#include <boost/geometry/geometries/geometries.hpp>

namespace boost_geo = boost::geometry;
namespace trans = boost::geometry::strategy::transform;
using namespace boost::polygon;
using boost::polygon::voronoi_builder;
using boost::polygon::voronoi_diagram;

typedef boost_geo::model::point<double, 2, boost_geo::cs::cartesian> Point;
typedef boost_geo::model::polygon<Point> Polygon;
typedef boost_geo::model::multi_polygon<Polygon> MultiPolygon;
typedef boost_geo::model::box<Point> Box;
typedef std::vector<std::pair<double, double>> PolygonInput;
typedef Point Vector;

const double EPS = 1e-8;
const double INF = 4e18;
const double PI = acos(-1);
const int MAX_PIECE = 3;
static int PATTERN_NO;
static int frameno;

int dblcmp(double d)
{
    if (fabs(d)<EPS)return 0;
    return d>EPS?1:-1;
}

bool vis[5000][10000];

namespace boost_geo_util
{
    Polygon constructBGPolygon(PolygonInput &);
    bool isPolygonIntersectPolygon(MultiPolygon &, MultiPolygon &);
    bool isPointInsidePolygons(MultiPolygon &, Point &);
    void visualize(MultiPolygon &, std::string);
}; // namespace boost_geo_util

namespace bin_packing
{
    void binPacking(std::vector<Polygon> &, double, double &, std::string);
    void placeItem(MultiPolygon &, Polygon &, double, double &);
    void normalize(PolygonInput &);
    void normalizePolygon(Polygon &);
    double getLength(Polygon &);
    double getWidth(Polygon &);
}; // namespace bin_packing

namespace pattern_cutting
{
    MultiPolygon triangulatePolygon(Polygon &);
    MultiPolygon splitTriangleByLine(Polygon &, Point, Point);
    MultiPolygon polygonUnion(MultiPolygon &);
    void patternCutting(Polygon, std::vector<Polygon>, int);
    MultiPolygon splitPolygonByLine(Polygon, Point, Point);
};

namespace geometry
{
    int dblcmp(double);
    double crossProduct(Point,Point);
    int orientationOfPointAroundLine(Point, Point, Point); 
    Point lineSegmentIntersectionPoint(Point, Point, Point, Point);
}; 


namespace no_fit_polygon
{
    MultiPolygon triangulatePolygon(Polygon&); 
    Polygon getNoFitPolygonOfTwoConvexPolygons(Polygon, Polygon);
    Polygon getNoFitPolygonOfTwoPolygons(Polygon, Polygon);
}; // namespace no_fit_polygon

void getNoFitPolygonOfTwoConvexPolygons_test1()
{
    MultiPolygon polygons;
    Polygon a({{
        Point(10,10),
        Point(15,15),
        Point(20,10),
        Point(10,10)
    }});
    Polygon b({{
        Point(0,0),
        Point(0,5),
        Point(5,0),
        Point(0,0)
    }});

    Polygon noFitPolygon = no_fit_polygon::getNoFitPolygonOfTwoConvexPolygons(a, b);
    polygons.push_back(noFitPolygon);
    polygons.push_back(a);
    for(auto point : noFitPolygon.outer())
    {
        Polygon c = b;
        for(auto &p : c.outer())
        {
            p = Point(point.get<0>() + p.get<0>(), point.get<1>() + p.get<1>());
        }
       // polygons.push_back(c);
    }
    boost_geo_util::visualize(polygons, "getNoFitPolygonOfTwoConvexPolygons_test1");
}
void getNoFitPolygonOfTwoConvexPolygons_test2()
{
    MultiPolygon polygons;
    Polygon a({{
        Point(2,2),
        Point(2,8),
        Point(8,8),
        Point(13,5),
        Point(8,2),
        Point(2,2)
    }});
    Polygon b({{
        Point(0,0),
        Point(0,6),
        Point(4,0),
        Point(0,0)
    }});

    Polygon noFitPolygon = no_fit_polygon::getNoFitPolygonOfTwoConvexPolygons(a, b);
    polygons.push_back(noFitPolygon);
    polygons.push_back(a);
    for(auto point : noFitPolygon.outer())
    {
        Polygon c = b;
        for(auto &p : c.outer())
        {
            p = Point(point.get<0>() + p.get<0>(), point.get<1>() + p.get<1>());
        }
       // polygons.push_back(c);
    }
    boost_geo_util::visualize(polygons, "getNoFitPolygonOfTwoConvexPolygons_test2");
}
void getNoFitPolygonOfTwoConvexPolygons_test3()
{
    MultiPolygon polygons;
    Polygon a({{
        Point(0,0),
        Point(0,5),
        Point(5,10),
        Point(10,10),
        Point(10,5),
        Point(5,0),
        Point(0,0)
    }});
    Polygon b({{
        Point(0,0),
        Point(0,5),
        Point(5,5),
        Point(5,0),
        Point(0,0)
    }});

    Polygon noFitPolygon = no_fit_polygon::getNoFitPolygonOfTwoConvexPolygons(a, b);
    polygons.push_back(noFitPolygon);
    polygons.push_back(a);
    for(auto point : noFitPolygon.outer())
    {
        Polygon c = b;
        for(auto &p : c.outer())
        {
            p = Point(point.get<0>() + p.get<0>(), point.get<1>() + p.get<1>());
        }
       // polygons.push_back(c);
    }
    boost_geo_util::visualize(polygons, "getNoFitPolygonOfTwoConvexPolygons_test3");
}
int main()
{
    getNoFitPolygonOfTwoConvexPolygons_test1();
    getNoFitPolygonOfTwoConvexPolygons_test2();
    getNoFitPolygonOfTwoConvexPolygons_test3();
    return 0;
}
void boost_geo_util::visualize(MultiPolygon &multipolygon, std::string datasetName)
{
    Box box;
    boost::geometry::envelope(multipolygon, box);
    std::ostringstream name;
    name << datasetName << ".svg";
    std::ofstream svg(name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:1", 5);
    mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(255,128,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
}
int geometry::dblcmp(double d)
{
    if (fabs(d)<EPS)return 0;
    return d>EPS?1:-1;
}
MultiPolygon no_fit_polygon::triangulatePolygon(Polygon &polygon)
{
    MultiPolygon triangles;
    std::vector<Point> vertices;

    for (auto point : polygon.outer())
    {
        vertices.push_back(point);
    }

    while (vertices.size() > 3)
    {
        bool okay = false;
        int n = vertices.size();
        for (int i = 1; i + 1 < n; i += 1)
        {
            Polygon triangle({{vertices[i - 1], vertices[i], vertices[i + 1], vertices[i - 1]}});

            MultiPolygon intersections;
            boost_geo::intersection(polygon, triangle, intersections);
            if (geometry::dblcmp(std::fabs(boost_geo::area(intersections)) - std::fabs(boost_geo::area(triangle))) == 0)
            {
                okay = true;
                triangles.push_back(triangle);
                std::vector<Point> t;
                for (int j = 0; j < n; j += 1)
                {
                    if (i != j)
                    {
                        t.push_back(vertices[j]);
                    }
                }
                vertices.clear();
                vertices = t;
                t.clear();
                break;
            }
        }
        assert(okay == true);
    }
    assert(triangles.size() == polygon.outer().size() - 3);
    return triangles;
}
Polygon no_fit_polygon::getNoFitPolygonOfTwoConvexPolygons(Polygon convexPolygon1, Polygon convexPolygon2)
{
    std::vector<Point> points;

    // make convexPolygon1 anti-clockwise
    for(int i = convexPolygon1.outer().size() - 1; i - 1 >= 0; i -= 1)
    {
        Point a = convexPolygon1.outer()[i];
        Point b = convexPolygon1.outer()[i - 1];
        points.push_back(Point(b.get<0>() - a.get<0>(), b.get<1>() - a.get<1>()));
    }
    
    // make convexPolygon2 clockwise
    for(int i = 0; i + 1 < convexPolygon2.outer().size(); i += 1)
    {
        Point a = convexPolygon2.outer()[i];
        Point b = convexPolygon2.outer()[i + 1];
        points.push_back(Point(b.get<0>() - a.get<0>(), b.get<1>() - a.get<1>()));
    }
    // sort vectors by angle
    sort(points.begin(), points.end(), [](Point a, Point b) {
        double x1 = a.get<0>(), y1 = a.get<1>();
        double x2 = b.get<0>(), y2 = b.get<1>();
        double angle1 = atan2(y1, x1);
        double angle2 = atan2(y2, x2);
        if( angle1 < 0 ) angle1 += 2.0 * PI; 
        if( angle2 < 0 ) angle2 += 2.0 * PI; 
        
        if( angle1 < angle2 ) return true;
        if( geometry::dblcmp(angle1-angle2) == 0 ) return x1 > x2;
        return false;
    });


    Polygon noFitPolygon;
    std::vector<Point> t{Point(0,0)};
    // make no fit polygon
    for(int i = 0; i < points.size(); i += 1)
    {   
        t.push_back(Point(points[i].get<0>() + t.back().get<0>(), 
                          points[i].get<1>() + t.back().get<1>()));
    }

    double y1 = 0, y2 = 0, x1 = 0, x2 = 0;
    for(auto point : convexPolygon1.outer())
    {
        y1 = std::max(y1, point.get<1>());   
        x1 = std::max(x1, point.get<0>());   
    }
    for(int i = t.size() - 1; i >= 0; i -= 1)
    {
        noFitPolygon.outer().push_back(Point(t[ i ].get<0>() + convexPolygon1.outer()[0].get<0>(),
                                             t[ i ].get<1>() + convexPolygon1.outer()[0].get<1>()));
        y2 = std::max(y2, noFitPolygon.outer().back().get<1>()); 
        x2 = std::max(x2, noFitPolygon.outer().back().get<0>()); 
        std::cout << noFitPolygon.outer().back().get<0>() << " " << noFitPolygon.outer().back().get<1>() << "\n";
        //std::cout << t[i].get<0>() << " " << t[i].get<1>() << "\n";
    }
    for(auto &point : noFitPolygon.outer())
    {
        point = Point(point.get<0>() - x2 + x1, point.get<1>() - y2 + y1);
    }
    return noFitPolygon;
}
