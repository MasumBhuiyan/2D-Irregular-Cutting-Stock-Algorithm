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
typedef boost::geometry::model::linestring<Point> Line;
typedef boost_geo::model::polygon<Point> Polygon;
typedef boost_geo::model::multi_polygon<Polygon> MultiPolygon;
typedef boost_geo::model::box<Point> Box;
typedef Point Vector;

#define x get<0>()
#define y get<1>()

const double EPS = 1e-8;
const double INF = 4e18;
const double PI = acos(-1);

static int frameno;

namespace polygon_fit 
{
    // level 0
    Polygon noFitPolygon(Polygon&, Polygon&);
    Polygon innerFitPolygon(Polygon&, double, double);

    // level 1
    void allPairNoFitPolygons(std::vector<Polygon>&);
    void allInnerFitPolygons(std::vector<Polygon>&, double, double);
};

namespace polygon_util
{
    void concavityOfAllNoFitPolygons(std::vector<Polygon>&);
    Polygon convexHull(std::vector<Point>&); 
    std::vector<Polygon> findConvexHullVacancy(Polygon&);
    Point findDominantPoint(Polygon&);
    void findAllPairDominantPoint(std::vector<Polygon>&);
    void findAllConvexHullVacancies(std::vector<Polygon>&);
    double calulateClusteringCriteria1(Polygon&, Polygon&);
    double calulateClusteringCriteria2(Polygon&, Polygon&);
    double calulateClusterValue(Polygon&, Polygon&);
}
namespace initial_solution
{
    // level 0
    void sort(std::vector<std::vector<Polygon>>&);
    Point findBlfPoint(std::vector<Polygon>&, std::vector<Polygon>&);
    std::tuple<std::vector<Point>, std::vector<double>> perfectMatching(double);

    // level 1
    std::vector<Polygon> blf(std::vector<std::vector<Polygon>>&);
    std::tuple<std::vector<Point>, std::vector<double>> cluster(std::vector<Polygon>&);
    std::tuple<std::vector<Point>, std::vector<double>> generateInitialSolution(std::vector<Polygon>&, double);
};
namespace overlap_minimization
{

};
namespace cuckoo_search
{

};
namespace bin_packing
{
    std::tuple<std::vector<Polygon>, double> readDataset(std::string);
    std::tuple<std::vector<Polygon>> gcs(std::vector<Polygon>&, double);
};
#endif