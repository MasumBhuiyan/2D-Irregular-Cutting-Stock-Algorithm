#include <_2D_irregular_bin_packing.hpp>

void dnc_approach::mergeHeuristics1Placement(
    MultiPolygon &multiPolygon_a, MultiPolygon &multiPolygonB,
    Point p, Point q, Point r, Point s,
    double &minAreaOfMBR, MultiPolygon &bestOrientation)
{
    Vector pq = p;
    boost_geo::subtract_point(pq, q);
    Vector rs = r;
    boost_geo::subtract_point(rs, s);
    // find directed angle difference CCW
    double angleDiff = boost_geo_util::getDirectedAngle_D(pq, rs);
    // rotate multipolygonB by angleDiff CW to aligin the edge with respect to r
    MultiPolygon tmpMultiPolygon, multiPolygon_b;
    boost_geo::transform(multiPolygonB, tmpMultiPolygon, trans::translate_transformer<double, 2, 2>(-(r.x), -(r.y)));
    boost_geo::transform(tmpMultiPolygon, multiPolygon_b, trans::rotate_transformer<boost_geo::degree, double, 2, 2>(angleDiff));
    // translate multiPolygon_b to point p so that point r is upon p
    Point r_new = p;
    boost_geo::subtract_point(r_new, r); // translation vector
    tmpMultiPolygon = multiPolygon_b;
    boost_geo::transform(tmpMultiPolygon, multiPolygon_b, trans::translate_transformer<double, 2, 2>(r_new.x, r_new.y)); // translate
    if (boost_geo_util::isPolygonIntersectPolygon(multiPolygon_a, multiPolygon_b) == false)
    {
        MultiPolygon combinedMultiPolygon = boost_geo_util::unionPolygons(multiPolygon_a, multiPolygon_b);
        Box minBoundRectangle;
        boost_geo::envelope(combinedMultiPolygon, minBoundRectangle);
        double minBoundRectangleArea = std::fabs(boost_geo::area(minBoundRectangle));
        if (minBoundRectangleArea < minAreaOfMBR)
        {
            minAreaOfMBR = minBoundRectangleArea;
            bestOrientation = combinedMultiPolygon;
        }
    }
    // reflect multipolygon_b with respect to pq
    MultiPolygon multiPolygon_b_reflected = boost_geo_util::reflectAcrossLine(multiPolygon_b, p, q);
    if (boost_geo_util::isPolygonIntersectPolygon(multiPolygon_a, multiPolygon_b_reflected) == false)
    {
        MultiPolygon combinedMultiPolygon = boost_geo_util::unionPolygons(multiPolygon_a, multiPolygon_b_reflected);
        Box minBoundRectangle;
        boost_geo::envelope(combinedMultiPolygon, minBoundRectangle);
        double minBoundRectangleArea = std::fabs(boost_geo::area(minBoundRectangle));
        if (minBoundRectangleArea < minAreaOfMBR)
        {
            minAreaOfMBR = minBoundRectangleArea;
            bestOrientation = combinedMultiPolygon;
        }
    }
}

/**
 * first heuristics method for placement
*/
MultiPolygon dnc_approach::mergeHeuristics1(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    if (std::fabs(boost_geo::area(multiPolygonA)) < std::fabs(boost_geo::area(multiPolygonB)))
    {
        std::swap(multiPolygonA, multiPolygonB);
    }
    double minAreaOfMBR = INF; // MBR = Minimum Bound Rectangle of a ploygon
    MultiPolygon bestOrientation;
    int angleInterval = 90;
    for (int angle_i = 0; angle_i < 360; angle_i += angleInterval)
    {
        MultiPolygon multiPolygon_a;
        trans::rotate_transformer<boost_geo::degree, double, 2, 2> rotate(angle_i);
        boost_geo::transform(multiPolygonA, multiPolygon_a, rotate);
        for (Polygon &polygon_ai : multiPolygon_a)
        {
            std::vector<Point> polygon_ai_outerRing = polygon_ai.outer();
            int n = polygon_ai_outerRing.size();
            for (int i = 0; i + 1 < n; i++)
            {
                Point p = polygon_ai_outerRing[i]; // each edge of polygonA
                Point q = polygon_ai_outerRing[i + 1];
                for (Polygon &polygon_bi : multiPolygonB)
                {
                    std::vector<Point> polygon_bi_outerRing = polygon_bi.outer();
                    int m = polygon_bi_outerRing.size();
                    for (int j = 0; j + 1 < m; j++)
                    {
                        Point r = polygon_bi_outerRing[j]; // each edge of polygonB
                        Point s = polygon_bi_outerRing[j + 1];
                        mergeHeuristics1Placement(multiPolygon_a, multiPolygonB, p, q, r, s, minAreaOfMBR, bestOrientation);
                        mergeHeuristics1Placement(multiPolygon_a, multiPolygonB, p, q, s, r, minAreaOfMBR, bestOrientation);
                        mergeHeuristics1Placement(multiPolygon_a, multiPolygonB, q, p, r, s, minAreaOfMBR, bestOrientation);
                        mergeHeuristics1Placement(multiPolygon_a, multiPolygonB, q, p, s, r, minAreaOfMBR, bestOrientation);
                    }
                }
            }
        }
    }
    assert(minAreaOfMBR != INF);
    return bestOrientation;
}

/**
 * merge
 */
MultiPolygon dnc_approach::mergeMultiPolygons(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    return mergeHeuristics1(multiPolygonA, multiPolygonB);
}

/**
 * divide and conquer
 */
MultiPolygon dnc_approach::split(std::vector<Polygon> &polygons, int left, int right)
{
    if (left == right)
    {
        return MultiPolygon({polygons[left]});
    }
    int mid = left + (right - left) / 2;

    MultiPolygon multiPolygonLeft = split(polygons, left, mid);
    MultiPolygon multiPolygonRight = split(polygons, mid + 1, right);

    return mergeMultiPolygons(multiPolygonLeft, multiPolygonRight);
}

void dnc_approach::solution(std::vector<PolygonInput> &items)
{
    std::cout << std::fixed << std::setprecision(3);
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfItems = items.size();
    std::cout << "number of Items: " << numberOfItems << std::endl;
    std::vector<Polygon> polygons(numberOfItems);
    double totalAreaOfItems = 0;
    for (int i = 0; i < numberOfItems; i++)
    {
        polygons[i] = boost_geo_util::constructBGPolygon(items[i]);
        totalAreaOfItems += boost_geo::area(polygons[i]);
    }

    MultiPolygon resultMultiPolygon = split(polygons, 0, numberOfItems - 1);
    double resultMultiPolygonArea = boost_geo::area(resultMultiPolygon);
    Box stock;
    boost_geo::envelope(resultMultiPolygon, stock);
    double stockArea = boost_geo::area(stock);
    double packingDensity = (totalAreaOfItems / stockArea) * 100;
    Point stockDimension = stock.max_corner();
    boost_geo::subtract_point(stockDimension, stock.min_corner());

    std::cout << "total area of items........: " << totalAreaOfItems << std::endl;
    std::cout << "result polygon set area....: " << resultMultiPolygonArea << std::endl;
    std::cout << "stock dimension [l * w]....: "
              << "[" << std::fabs(stockDimension.x) << ", " << std::fabs(stockDimension.y) << "]" << std::endl;
    std::cout << "stock area.................: " << stockArea << std::endl;
    std::cout << "packing density............: " << packingDensity << " %" << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function.....: " << duration.count() / 1000000.0 << " seconds" << std::endl;

    std::cout << boost_geo::wkt(resultMultiPolygon) << std::endl;
    boost_geo_util::visualize(resultMultiPolygon);
}