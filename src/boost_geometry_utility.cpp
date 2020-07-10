#include <boost_geometry_utility.hpp>

double boost_geo_util::getDirectedAngle_D(Vector vector1, Vector vector2)
{
    double angle = atan2(vector2.y, vector2.x) - atan2(vector1.y, vector1.x);
    if (angle < 0)
    {
        angle += 2 * PI;
    }
    return (angle * 180) / PI;
}

Polygon boost_geo_util::constructBGPolygon(PolygonInput &polygonFromInput)
{
    int n = polygonFromInput.size();
    assert(n >= 3);
    Polygon polygonBG;
    auto startPoint = polygonFromInput[0];
    boost_geo::append(polygonBG.outer(), Point(startPoint.first, startPoint.second));
    std::reverse(polygonFromInput.begin(), polygonFromInput.end()); // make it clockwise
    for (auto point_i : polygonFromInput)
    {
        boost_geo::append(polygonBG.outer(), Point(point_i.first, point_i.second));
    }
    return polygonBG;
}

MultiPolygon boost_geo_util::unionPolygons(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    MultiPolygon multiPolygonUnion;
    for (Polygon polygon : multiPolygonA)
        multiPolygonUnion.push_back(polygon);
    for (Polygon polygon : multiPolygonB)
        multiPolygonUnion.push_back(polygon);
    return multiPolygonUnion;
}

MultiPolygon boost_geo_util::reflectAcrossLine(MultiPolygon &multiPolygon, Point p, Point q)
{
    double x1 = p.x, x2 = q.x;
    double y1 = p.y, y2 = q.y;
    MultiPolygon reflected = multiPolygon;
    for (Polygon &polygon : multiPolygon)
    {
        for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
        {
            Point tmpPoint = *it;
            if ((x2 - x1) < EPS)
            {
                *it = Point(2.0 * x1 - tmpPoint.x, tmpPoint.y);
            }
            else if ((y2 - y1) < EPS)
            {
                *it = Point(tmpPoint.x, 2.0 * y1 - tmpPoint.y);
            }
            else
            {
                double m = (y2 - y1) / (x2 - x1);
                double b = y1 - m * x1;

                *it = Point(((1 - m * m) * (tmpPoint.x) + 2 * m * (tmpPoint.y) - 2 * m * b) / (m * m + 1),
                            ((m * m - 1) * (tmpPoint.y) + 2 * m * (tmpPoint.x) + 2 * b) / (m * m + 1));
            }
        }
    }
    return reflected;
}

bool boost_geo_util::isPolygonIntersectPolygon(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    for (auto polygon_a : multiPolygonA)
    {
        for (auto polygon_b : multiPolygonB)
        {
            std::deque<Polygon> output;
            boost_geo::intersection(polygon_a, polygon_b, output);
            if (output.size() > 0)
                return true;
        }
    }
    return false;
    //return boost_geo::overlaps(multiPolygonA, multiPolygonB);
}

void boost_geo_util::visualize(MultiPolygon &multipolygon)
{
    std::cout << std::fixed << std::setprecision(3);
    Box box;
    boost::geometry::envelope(multipolygon, box);
    std::cout << "make_envelope:" << boost::geometry::dsv(box) << std::endl;
    std::ostringstream name;
    name << "frame" << std::setw(4) << std::setfill('0') << frameno++ << ".svg";
    std::ofstream svg(name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:1", 5);
    mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(255,128,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
}