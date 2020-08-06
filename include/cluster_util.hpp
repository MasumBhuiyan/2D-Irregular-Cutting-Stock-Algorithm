#ifndef CLUSTER_UTIL
#define CLUSTER_UTIL

#include <bin_packing.hpp>
#include <geo_util.hpp>
#include <polygon_fit.hpp>

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
    std::vector<Point> getCandidatePlacementPositions(std::vector<Polygon> &, std::vector<Polygon> &);
    Point findBlfPoint(std::vector<Polygon> &, std::vector<Polygon> &);
    std::vector<Polygon> blf(std::vector<std::vector<Polygon>> &);
    double getBestClusters(std::vector<std::vector<double>> &, std::vector<double> &, int, int);
    void printBestClusters(std::vector<std::vector<double>> &, std::vector<double> &, int, int, std::vector<std::tuple<int, int>> &);
    std::vector<std::tuple<int, int>> perfectClustering(std::vector<std::vector<double>> &, double);
    std::vector<std::vector<double>> getClusterValues(std::vector<Polygon> &);
    std::vector<Polygon> generateInitialSolution(std::vector<Polygon> &, double);
}; // namespace cluster_util

#endif