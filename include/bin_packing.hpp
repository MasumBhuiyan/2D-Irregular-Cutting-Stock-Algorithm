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

using std::tuple;
using std::vector;

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
const double INITIAL_STOCK_LENGTH = 100000;
const int MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA = 40;
const vector<double> ALLOWABLE_ROTATIONS = {0, 90, 180, 270};

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
    vector<Polygon> rotatePolygons(vector<Polygon> &, Point, double);
    MultiPolygon translatePolygons(MultiPolygon cluster, Point translationPoint);
    bool isItPossibleToPlacePolygon(vector<Polygon> &, vector<Polygon>, Point);
    bool pointInRectangle(Point, Point, Point);
    double getPackingLength(MultiPolygon &);
    Polygon makePolygon(Polygon, Point, double);
    void visualize(MultiPolygon &, std::string);
}; // namespace geo_util

namespace polygon_fit
{
    Polygon getInnerFitRectangle(MultiPolygon cluster, double length, double width);
    vector<Point> getAllEdgeIntersectionPoints(MultiPolygon &allNfpIfr);
    MultiPolygon getNoFitPolygon(Polygon &referencePolygon, MultiPolygon &cluster);
    MultiPolygon getAllNfpIfr(MultiPolygon &packing, MultiPolygon cluster, double length, double width); // Nfp = No Fit Polygon, Ifr = Inner Fit Rectangle
};                                                                                                       // namespace polygon_fit

namespace cluster_util
{
    Polygon convexHull(MultiPolygon multiPolygon);
    MultiPolygon findConvexHullVacancy(Polygon &concavePolygon);
    vector<tuple<Point, Point>> findOppositeSideOfVacancies(Polygon &concavePolygonConvexHull, MultiPolygon &convexHullVacancies);
    Point findDominantPoint(Polygon &concavePolygon);
    double getClusteringCriteria1(Polygon &polygon1, Polygon &polygon2);
    double getClusteringCriteria2(Polygon &polygon1, Polygon &polygon2);
    double getClusterValue(Polygon &polygon1, Polygon &polygon2);
    void sortByClusterValue(vector<MultiPolygon> &clusters);
    vector<Point> getCandidatePlacementPositions(MultiPolygon &packing, MultiPolygon &cluster, double length, double width);
    Point findBLFPoint(MultiPolygon &packing, MultiPolygon &cluster, double length, double width); // BLF = Bottom Left fill
    MultiPolygon bottomLeftFill(vector<MultiPolygon> &clusters, double length, double width);
    double findBestPairWiseClusters(vector<vector<vector<vector<double>>>> &clusterValues, vector<double> &dp, int numberOfPairs, int mask);
    void printBestPairWiseClusters(vector<vector<vector<vector<double>>>> &clusterValues, vector<double> &dp, int numberOfPairs, int mask, vector<tuple<int, int, int, int>> &clusterPairs);
    vector<tuple<int, int, int, int>> getPerfectClustering(vector<vector<vector<vector<double>>>> &clusterValues, double width);
    vector<vector<vector<vector<double>>>> getClusterValues(vector<Polygon> &inputPolygons);
    MultiPolygon generateInitialSolution(vector<Polygon> &inputPolygons, double width);
}; // namespace cluster_util

namespace bin_packing
{
    tuple<vector<Polygon>, double> readDataset(std::string);
    bool isFeasible(MultiPolygon &, double);
    double getPenetrationDepth(Polygon, Polygon);
    double getTotalPenetrationDepth(MultiPolygon &);
    double getOverlapPenalty(MultiPolygon &, vector<vector<double>> &, int, double, Point);
    void increasePenalty(MultiPolygon &, vector<vector<double>> &);
    Point cuckooSearch(MultiPolygon &, vector<vector<double>> &, int, double, double, double);
    MultiPolygon minimizeOverlap(MultiPolygon, vector<double>, double, double);
    void binPacking(vector<Polygon> &, double, std::string, std::string, double runTimeDuration = RUN_TIME_DURATION);
}; // namespace bin_packing

#endif
