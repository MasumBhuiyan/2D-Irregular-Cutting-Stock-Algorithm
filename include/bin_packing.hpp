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
#include <stdlib.h>
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
const long double NFP_TRANSLATOR_COF = 1e4;
const long double INITIAL_STOCK_LENGTH = 100000000;
const int MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA = 40;
const vector<long double> ALLOWABLE_ROTATIONS = {0, 90, 180, 270};

static int frameno;

namespace geo_util
{
    int dblcmp(long double, long double eps = EPS);
    long double dblround(long double, long double eps = EPS);
    long double crossProduct(Point, Point);
    int orientation(Point, Point, Point);
    long double linePointDistance(Point, Point, Point);
    Point segmentSegmentIntersectionPoint(Point, Point, Point, Point);
    bool pointInRectangle(Point, Point, Point);

    namespace poly_util
    {
        void polygonRound(Polygon &polygon);
        Polygon normalize(Polygon &polygon);
        Polygon translate(Polygon &polygon, Point translationPoint);
        MultiPolygon translate(MultiPolygon &polygons, Point translationPoint);
        Polygon rotateCW(Polygon &polygon, long double rotationAngleInDegree, Point referencePoint = Point(0, 0));
        MultiPolygon rotateCW(MultiPolygon multiPolygon, long double rotationAngleInDegree, Point referencePoint = Point(0,0));
        long double polygonPolygonIntersectionArea(Polygon &, Polygon &);
        bool isItPossibleToPlacePolygon(MultiPolygon &packing, MultiPolygon clusterNextToBePlaced, Point translationPoint);
        long double getWidth(MultiPolygon &multiPolygon);
        long double getLength(MultiPolygon &multiPolygon);
    }; // namespace poly_util
    void visualize(MultiPolygon multiPolygon, string outputLocation, string datasetName);
    void visualizeCluster(Polygon polygon, MultiPolygon cluster, Polygon nfp, std::string location, int testId);

}; // namespace geo_util

namespace polygon_fit
{
    Polygon getInnerFitRectangle(MultiPolygon cluster, long double length, long double width);
    vector<Point> getAllEdgeIntersectionPoints(MultiPolygon &allNfpIfr);
    Polygon getNoFitPolygon(Polygon referencePolygon, MultiPolygon cluster);
    Polygon getNoFitPolygon(Polygon referencePolygon, Polygon polygonToPlace);
    MultiPolygon getAllNfpIfr(MultiPolygon &packing, MultiPolygon cluster, long double length, long double width); // Nfp = No Fit Polygon, Ifr = Inner Fit Rectangle

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
    vector<tuple<int, int, int, int>> getPerfectClustering(vector<vector<vector<vector<long double>>>> &clusterValues);
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
    void cuckooPacking(MultiPolygon initialPacking, long double runTimeDuration = RUN_TIME_DURATION);
    void binPacking(vector<Polygon> &polygons, long double width, string outputLocation, string datasetName, long double runTimeDuration = RUN_TIME_DURATION);
}; // namespace bin_packing

#endif
