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
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
//#include <libnfporb.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

namespace boost_geo = boost::geometry;
namespace trans = boost::geometry::strategy::transform;
//using namespace libnfporb;

typedef boost_geo::model::point<double, 2, boost_geo::cs::cartesian> Point;
typedef boost::geometry::model::linestring<Point> Line;
typedef boost_geo::model::polygon<Point> Polygon;
typedef boost_geo::model::multi_polygon<Polygon> MultiPolygon;
typedef boost_geo::model::box<Point> Box;
typedef Point Vector;

const double EPS = 1e-8;
const double INF = 4e18;
const double PI = acos(-1);
const double RUN_TIME_DURATION = 20;
const double FEASIBILTY = 1e-2;
const int MAXIMUM_GENERATION = 20;
const int NUMBER_OF_HOST_NESTS = 20;
const int MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA = 40;
const std::vector<double> ALLOWABLE_ROTATIONS = {0, 90, 180, 270};

static int frameno;

namespace geo_util
{
    int dblcmp(double, double eps = EPS);
    bool isConcave(Polygon &);
    double getLength(Polygon &);
    double getWidth(Polygon &);
    double crossProduct(Point, Point);
    void normalize(Polygon &);
    int orientation(Point, Point, Point);
    Polygon rotatePolygon(Polygon, Point, double);
    Polygon translatePolygon(Polygon, Point);
    double linePointDistance(Point, Point, Point);
    double polygonPolygonIntersectionArea(Polygon &, Polygon &);
    Point segmentSegmentIntersectionPoint(Point, Point, Point, Point);
    std::vector<Polygon> rotatePolygons(std::vector<Polygon> &, Point, double);
    std::vector<Polygon> translatePolygons(std::vector<Polygon>, Point);
    bool isItPossibleToPlacePolygon(std::vector<Polygon> &, std::vector<Polygon>, Point);
    bool pointInRectangle(Point, Point, Point);
    double getPackingLength(MultiPolygon &);
    Polygon makePolygon(Polygon, Point, double);
    void visualize(MultiPolygon &, std::string);
}; // namespace geo_util

namespace polygon_fit
{
    Polygon getInnerFitRectangle(std::vector<Polygon>, double, double);
    Polygon getNoFitPolygon(Polygon &, std::vector<Polygon>);
    MultiPolygon getAllNfpIfr(std::vector<Polygon> &, std::vector<Polygon>, double, double);
    std::vector<Point> getAllEdgeIntersectionPoints(std::vector<Polygon> &);
}; // namespace polygon_fit

namespace cluster_util
{
    Polygon convexHull(MultiPolygon);
    std::vector<Polygon> findConvexHullVacancy(Polygon &);
    std::vector<std::vector<Point>> findOppositeSideOfVacancies(Polygon &, std::vector<Polygon> &);
    Point findDominantPoint(Polygon &);
    std::vector<std::vector<Point>> findAllPairDominantPoint(std::vector<std::vector<Polygon>> &);
    std::vector<std::vector<Polygon>> findAllConvexHullVacancies(std::vector<Polygon> &);
    double clusteringCriteria1(Polygon &, Polygon &);
    double clusteringCriteria2(Polygon &, Polygon &);
    double getClusterValue(Polygon &, Polygon &);
    void sort(std::vector<std::vector<Polygon>> &);
    std::vector<Point> getCandidatePlacementPositions(std::vector<Polygon> &, std::vector<Polygon> &, double, double);
    Point findBlfPoint(std::vector<Polygon> &, std::vector<Polygon> &, double, double);
    std::vector<Polygon> blf(std::vector<std::vector<Polygon>> &, double, double);
    double getBestClusters(std::vector<std::vector<std::vector<std::vector<double>>>> &, std::vector<double> &, int, int);
    void printBestClusters(std::vector<std::vector<std::vector<std::vector<double>>>> &, std::vector<double> &, int, int, std::vector<std::tuple<int, int, int, int>> &);
    std::vector<std::tuple<int, int, int, int>> perfectClustering(std::vector<std::vector<std::vector<std::vector<double>>>> &, double);
    std::vector<std::vector<std::vector<std::vector<double>>>> getClusterValues(std::vector<Polygon> &);
    MultiPolygon generateInitialSolution(std::vector<Polygon> &, double);
}; // namespace cluster_util

namespace bin_packing
{
    std::tuple<std::vector<Polygon>, double> readDataset(std::string);
    bool isFeasible(MultiPolygon &, double);
    double getPenetrationDepth(Polygon, Polygon);
    double getTotalPenetrationDepth(MultiPolygon &);
    double getOverlapPenalty(MultiPolygon &, std::vector<std::vector<double>> &, int, double, Point);
    void increasePenalty(MultiPolygon &, std::vector<std::vector<double>> &);
    Point cuckooSearch(MultiPolygon &, std::vector<std::vector<double>> &, int, double, double, double);
    MultiPolygon minimizeOverlap(MultiPolygon, std::vector<double>, double, double);
    void binPacking(std::vector<Polygon> &, double, std::string, std::string, double runTimeDuration = RUN_TIME_DURATION);
}; // namespace bin_packing

#endif
