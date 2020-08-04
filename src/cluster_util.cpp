#include <cluster_util.hpp>

Polygon cluster_util::convexHull(MultiPolygon multiPolygon)
{
    Polygon convexhull;
    boost_geo::convex_hull(multiPolygon, convexhull);
    return convexhull;
}
std::vector<Polygon> cluster_util::findConvexHullVacancy(Polygon &polygon)
{
    std::vector<Polygon> vacancies;
    Polygon convexhull = cluster_util::convexHull({polygon});
    boost_geo::difference(convexhull, polygon, vacancies);
    return vacancies;
}
std::vector<std::vector<Point>> cluster_util::findOppositeSideOfVacancies(Polygon &convexhull, std::vector<Polygon> &vacancies)
{
    std::vector<std::vector<Point>> oppositeSideOfVacancies;
    for(auto vacancy: vacancies)
    {
        std::vector<Point> line;
        assert( boost_geo::intersection(convexhull, vacancy, line) == true );
        oppositeSideOfVacancies.push_back(line);
    }
    return oppositeSideOfVacancies;
}
Point cluster_util::findDominantPoint(Polygon &polygon)
{
    Point dominant;
    Polygon convexhull = cluster_util::convexHull({polygon});
    std::vector<Polygon> vacancies = cluster_util::findConvexHullVacancy(polygon);
    std::vector<std::vector<Point>> oppositeSideOfVacancies = cluster_util::findOppositeSideOfVacancies(convexhull, vacancies);

    int n = oppositeSideOfVacancies.size();
    assert( vacancies.size() == oppositeSideOfVacancies.size() );
    double maxDistance = 0.0;

    for(int i = 0; i < n; i += 1)
    {
        for(Point point: vacancies[ i ].outer())
        {
            double distance = geo_util::linePointDistance(oppositeSideOfVacancies[i][0], oppositeSideOfVacancies[i][1], point);
            if( distance > maxDistance )
            {
                maxDistance = distance;
                dominant = point;
            }
        }
    }
    return dominant;
}
std::vector<std::vector<Point>> cluster_util::findAllPairDominantPoint(std::vector<std::vector<Polygon>> &noFitPolygons)
{
    int n = noFitPolygons.size();
    std::vector<std::vector<Point>> points(n, std::vector<Point>(n));
    for(int i = 0; i < n; i += 1)
    {
        for(int j = 0; j < n; j += 1)
        {
            if( i != j )
            {
                points[ i ][ j ] = cluster_util::findDominantPoint(noFitPolygons[ i ][ j ]);
            }
        }
    }
    return points;
}
std::vector<std::vector<Polygon>> cluster_util::findAllConvexHullVacancies(std::vector<Polygon> &polygons)
{
    std::vector<std::vector<Polygon>> allConvexHullVacancies;
    for(auto &polygon: polygons)
    {
        allConvexHullVacancies.push_back(cluster_util::findConvexHullVacancy(polygon));
    }
    return allConvexHullVacancies;
}
double cluster_util::clusteringCriteria1(Polygon &polygon1, Polygon &polygon2)
{
    double value = 0.0, value1 = 0.0, value2 = 0.0;
    double areaIntersection1 = 0.0, totalVacancy1 = 0.0; 
    double areaIntersection2 = 0.0, totalVacancy2 = 0.0; 

    std::vector<Polygon> V1 = cluster_util::findConvexHullVacancy(polygon1);
    std::vector<Polygon> V2 = cluster_util::findConvexHullVacancy(polygon2);    

    for(auto vacancy: V2)
    {
        double area = geo_util::polygonPolygonIntersectionArea(polygon1, vacancy);
        if( area > 0 )
        {
            areaIntersection1 += area;
            totalVacancy1 += std::fabs(boost_geo::area(vacancy)); 
        }
    }
    for(auto vacancy: V1)
    {
        double area = geo_util::polygonPolygonIntersectionArea(polygon2, vacancy);
        if( area > 0 )
        {
            areaIntersection2 += area;
            totalVacancy2 += std::fabs(boost_geo::area(vacancy)); 
        }
    }
    if( totalVacancy1 > 0 ) value1 = areaIntersection1 / totalVacancy1;
    if( totalVacancy2 > 0 ) value2 = areaIntersection2 / totalVacancy2;
    value = std::max(value1, value2);
    return value;
}
double cluster_util::clusteringCriteria2(Polygon &polygon1, Polygon &polygon2)
{
    MultiPolygon multiPolygon;
    multiPolygon.push_back(polygon1);
    multiPolygon.push_back(polygon2);
    Polygon convexhull = cluster_util::convexHull(multiPolygon);

    double area1 = std::fabs(boost_geo::area(polygon1));
    double area2 = std::fabs(boost_geo::area(polygon2));
    double area3 = std::fabs(boost_geo::area(convexhull));
    assert(area3 > 0);

    double value = (area1 + area2) / area3;
    return value;
}
double cluster_util::getClusterValue(Polygon &polygon1, Polygon &polygon2)
{
    double value = cluster_util::clusteringCriteria1(polygon1, polygon2) + cluster_util::clusteringCriteria2(polygon1, polygon2);
    return value;
}

void cluster_util::sort(std::vector<std::vector<Polygon>> &clusters)
{
    sort(clusters.begin(), clusters.end(), [](std::vector<Polygon> cluster1, std::vector<Polygon> cluster2)
    {
        double area1 = 0.0, area2 = 0.0;
        for(auto polygon: cluster1) area1 += std::fabs(boost_geo::area(polygon));
        for(auto polygon: cluster2) area2 += std::fabs(boost_geo::area(polygon));
        return geo_util::dblcmp(area1 - area2, EPS) >= 0;
    });
}
Point cluster_util::findBlfPoint(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> &clusterNextToBePlaced)
{
    Point blfPoint;
    return blfPoint;
}
std::vector<Polygon> cluster_util::blf(std::vector<std::vector<Polygon>> &clusters)
{
    std::vector<Polygon> stock;
    return stock;
}
std::vector<std::vector<Polygon>> cluster_util::perfectClustering(std::vector<std::vector<double>> &clusterValues, double noOfclusterPairs)
{
    std::vector<std::vector<Polygon>> clusters;
    return clusters;
}
std::vector<std::vector<double>> cluster_util::getClusterValues(std::vector<Polygon> &polygons)
{   
    int n = polygons.size();
    std::vector<std::vector<double>> clusterValues(n, std::vector<double>(n, 0.0));
    for(int i = 0; i < n; i += 1)
    {
        for(int j = 0; j < n; j += 1)
        {
            if( i != j )
            {
                 Polygon nfp = polygon_fit::noFitPolygon(polygons[ i ], polygons[ j ]);
                 if( geo_util::isConcave(nfp) == true )
                 {
                     Point point = cluster_util::findDominantPoint(nfp);
                     Polygon polygon_j = geo_util::translatePolygon(polygons[ j ], point);
                     clusterValues[ i ][ j ] = cluster_util::getClusterValue(polygons[ i ], polygon_j);
                 }
            }
        }
    }
    return clusterValues;
}
std::vector<Polygon> cluster_util::generateInitialSolution(std::vector<Polygon> &polygons, double width)
{
    std::vector<std::vector<double>> clusterValues = cluster_util::getClusterValues(polygons);
    std::vector<std::vector<Polygon>> clusters = cluster_util::perfectClustering(clusterValues, std::min((int)polygons.size(), 10));
    cluster_util::sort(clusters);
    std::vector<Polygon> initSol = cluster_util::blf(clusters);
    return initSol;
}