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

using std::string;
using std::tuple;
using std::vector;

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/is_convex.hpp>

namespace boost_geo = boost::geometry;
namespace trans = boost::geometry::strategy::transform;

typedef boost_geo::model::point<long double, 2, boost_geo::cs::cartesian> Point;
typedef boost::geometry::model::linestring<Point> Line;
typedef boost_geo::model::polygon<Point, false> Polygon;
typedef boost_geo::model::multi_polygon<Polygon> MultiPolygon;
typedef boost_geo::model::box<Point> Box;
typedef Point Vector;

const long double EPS = 1e-8;
const long double INF = 4e18;
const long double PI = acos(-1);
const long double RUN_TIME_DURATION = 20;
const long double FEASIBILTY = 1e-2;
const int MAXIMUM_GENERATION = 20;
const int NUMBER_OF_HOST_NESTS = 20;
const long double INITIAL_STOCK_LENGTH = 100000;
const int MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA = 40;
const vector<long double> ALLOWABLE_ROTATIONS = {0, 90, 180, 270};

static int frameno;

namespace geo_util
{
    int dblcmp(long double, long double eps = EPS);
    bool isConcave(Polygon &);
    long double getLength(Polygon &);
    long double getWidth(Polygon &);
    long double crossProduct(Point, Point);
    void normalize(Polygon &);
    int orientation(Point, Point, Point);
    Polygon rotatePolygon(Polygon, Point, long double);
    Polygon translatePolygon(Polygon, Point);
    long double linePointDistance(Point, Point, Point);
    long double polygonPolygonIntersectionArea(Polygon &, Polygon &);
    Point segmentSegmentIntersectionPoint(Point, Point, Point, Point);
    vector<Polygon> rotatePolygons(vector<Polygon> &, Point, long double);
    MultiPolygon translatePolygons(MultiPolygon cluster, Point translationPoint);
    bool isItPossibleToPlacePolygon(MultiPolygon &packing, MultiPolygon clusterNextToBePlaced, Point translationPoint);
    bool pointInRectangle(Point, Point, Point);
    long double getPackingLength(MultiPolygon &);
    Polygon makePolygon(Polygon, Point, long double);
    namespace poly_util
    {
        Polygon normalize(Polygon &polygon);
        Polygon translate(Polygon &polygon, Point translationPoint);
        Polygon rotateCW(Polygon &polygon, long double rotationAngleInDegree, Point referencePoint = Point(0, 0));
    }; // namespace poly_util
    void visualize(MultiPolygon, string, string);

}; // namespace geo_util

namespace polygon_fit
{
    Polygon getInnerFitRectangle(MultiPolygon cluster, long double length, long double width);
    vector<Point> getAllEdgeIntersectionPoints(MultiPolygon &allNfpIfr);
    MultiPolygon getNoFitPolygon(Polygon referencePolygon, MultiPolygon cluster);
    MultiPolygon getNoFitPolygon(Polygon referencePolygon, Polygon polygonToPlace);
    MultiPolygon getAllNfpIfr(MultiPolygon &packing, MultiPolygon cluster, long double length, long double width); // Nfp = No Fit Polygon, Ifr = Inner Fit Rectangle
    void generateAllPairNfpForInputPolygons(string datasetname, int numberOfPolygons, string outputLocation);

}; // namespace polygon_fit

namespace cluster_util
{
    Polygon convexHull(MultiPolygon multiPolygon);
    MultiPolygon findConvexHullVacancy(Polygon &concavePolygon);
    vector<tuple<Point, Point>> findOppositeSideOfVacancies(Polygon &concavePolygonConvexHull, MultiPolygon &convexHullVacancies);
    Point findDominantPoint(Polygon &concavePolygon);
    long double getClusteringCriteria1(Polygon &polygon1, Polygon &polygon2);
    long double getClusteringCriteria2(Polygon &polygon1, Polygon &polygon2);
    long double getClusterValue(Polygon &polygon1, Polygon &polygon2);
    void sortByClusterValue(vector<MultiPolygon> &clusters);
    vector<Point> getCandidatePlacementPositions(MultiPolygon &packing, MultiPolygon &cluster, long double length, long double width);
    Point findBLFPoint(MultiPolygon &packing, MultiPolygon &cluster, long double length, long double width); // BLF = Bottom Left fill
    MultiPolygon bottomLeftFill(vector<MultiPolygon> &clusters, long double length, long double width);
    long double findBestPairWiseClusters(vector<vector<vector<vector<long double>>>> &clusterValues, vector<long double> &dp, int numberOfPairs, int mask);
    void printBestPairWiseClusters(vector<vector<vector<vector<long double>>>> &clusterValues, vector<long double> &dp, int numberOfPairs, int mask, vector<tuple<int, int, int, int>> &clusterPairs);
    vector<tuple<int, int, int, int>> getPerfectClustering(vector<vector<vector<vector<long double>>>> &clusterValues, long double width);
    vector<vector<vector<vector<long double>>>> getClusterValues(vector<Polygon> &inputPolygons);
    MultiPolygon generateInitialSolution(vector<Polygon> &inputPolygons, long double width);
}; // namespace cluster_util

namespace bin_packing
{
    tuple<vector<Polygon>, long double> readDataset(string datasetName);
    bool isFeasible(MultiPolygon &, long double);
    long double getPenetrationDepth(Polygon, Polygon);
    long double getTotalPenetrationDepth(MultiPolygon &);
    long double getOverlapPenalty(MultiPolygon &, vector<vector<long double>> &, int, long double, Point);
    void increasePenalty(MultiPolygon &, vector<vector<long double>> &);
    Point cuckooSearch(MultiPolygon &, vector<vector<long double>> &, int, long double, long double, long double);
    MultiPolygon minimizeOverlap(MultiPolygon, vector<long double>, long double, long double);
    void binPacking(vector<Polygon> &polygons, long double width, string outputLocation, string datasetName, long double runTimeDuration = RUN_TIME_DURATION);
}; // namespace bin_packing

#endif
