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
#include <sys/stat.h> 
#include <sys/types.h> 

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

/*
// Global variables
std::vector<std::vector<Point>> allPairDominantPoints;
std::vector<std::vector<Polygon>> allPairNoFitPolygons;
std::vector<std::vector<double>> allPairClusterValues;
std::vector<Polygon> allInnerFitPolygons;
std::vector<std::vector<Polygon>> convexHullVacancies;
std::vector<bool> convavity;
*/

namespace bin_packing
{
    std::tuple<std::vector<Polygon>, double> readDataset(std::string); // 1
    std::tuple<std::vector<Polygon>> gcs(std::vector<Polygon>&, double); // 2
    std::tuple<std::vector<Point>, std::vector<double>> generateInitialSolution(std::vector<Polygon>&, double); // 3
    std::tuple<std::vector<Point>, std::vector<double>> cluster(std::vector<Polygon>&); // 4
    void sort(std::vector<std::vector<Polygon>>&); // 5
    std::vector<Polygon> blf(std::vector<std::vector<Polygon>>&); // 6
    Polygon translatePolygon(Polygon&, Point); // 7
    std::vector<Polygon> translatePolygons(std::vector<Polygon>&, Point); // 8
    Polygon rotatePolygon(Polygon&, double); // 9
    std::vector<Polygon> rotatePolygons(std::vector<Polygon>&, double); // 10
    void calculateAllPairNoFitPolygons(std::vector<Polygon>&); // 11
    void calculateAllInnerFitPolygons(std::vector<Polygon>&, double, double); // 12
    Polygon calculateNoFitPolygon(Polygon&, Polygon&); // 13
    Polygon calculateInnerFitPolygon(Polygon&, double, double); // 14
    bool isConcave(Polygon&); // 15
    void calculateConcavityOfAllNoFitPolygons(std::vector<Polygon>&); // 16
    Point findDominantPoint(Polygon&); // 17
    std::vector<Polygon> findConvexHullVacancy(Polygon&); // 18
    double calulateClusteringCriteria1(Polygon&, Polygon&); // 19
    double calulateClusteringCriteria2(Polygon&, Polygon&); // 20
    double calulateClusterValue(Polygon&, Polygon&); // 21
    Polygon convexHull(std::vector<Point>&); // 22
    std::tuple<std::vector<Point>, std::vector<double>> perfectMatching(double); // 23 // cluster percentage
    Point findBlfPoint(std::vector<Polygon>&, std::vector<Polygon>&); // 24 // current all ready placed polygons, polygon or polygons next to be placed. 
                                                                      //find all the places this polygon/polygons can be place. basically all NFP-IFP intersection points and vertices
                                                                      // return the bottom left Point among the points
    bool isItPossibleToPlacePolygonAt(std::vector<Polygon>&, Polygon, Point); // 25 // current placed polygons, polygon next to be placed at Point
    double polygonPolygonIntersectionArea(Polygon&, Polygon&); // 26
    double linePointDistance(Point, Point, Point);// 27
};

#endif
