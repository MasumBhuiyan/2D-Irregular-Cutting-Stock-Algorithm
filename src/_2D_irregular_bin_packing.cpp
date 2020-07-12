#include <_2D_irregular_bin_packing.hpp>

/** namespace boost_geo_util */

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

void boost_geo_util::visualize(MultiPolygon &multipolygon, std::string prfx)
{
    Box box;
    boost::geometry::envelope(multipolygon, box);
    std::cout << "make_envelope:" << boost::geometry::dsv(box) << std::endl;
    std::ostringstream name;
    name << prfx << "frame" << std::setw(4) << std::setfill('0') << frameno++ << ".svg";
    std::ofstream svg("../diagrams/" + name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:1", 5);
    mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(255,128,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
}

bool boost_geo_util::isPointInsidePolygons(MultiPolygon &polygons, Point &point)
{
    for (auto poly : polygons)
    {
        if (boost::geometry::within(point, poly) == true)
            return true;
    }
    return false;
}

/** namespace zero_waste_apparels */
bool vis[5000][10000];

std::vector<PolygonInput> zero_waste_apparels::readDataset(std::string filename, std::vector<int> &quantity, double &width)
{
    std::vector<PolygonInput> items;
    std::ifstream file(filename);
    if (file.fail())
    {
        std::cerr << "file does not exists\n";
        exit(-1);
    }
    else
    {
        int n;
        file >> width;
        file >> n;
        quantity.resize(n);
        for (int i = 0; i < n; i += 1)
        {
            PolygonInput item;
            file >> quantity[i];
            int m;
            file >> m;
            while (m--)
            {
                double _x, _y;
                file >> _x >> _y;
                item.push_back({_x, _y});
            }
            zero_waste_apparels::normalize(item);
            for (int k = 0; k < quantity[i]; k += 1)
            {
                items.push_back(item);
            }
        }
    }
    file.close();
    return items;
}

void zero_waste_apparels::runDataset(std::string datasetName)
{
    double width;
    std::vector<int> quantity;
    std::vector<PolygonInput> items = readDataset(datasetName, quantity, width);
    /* for(auto &poly : items) {
        for(auto &point : poly) {
            point.first /= 2;
            point.second /= 2;
        }
    }
    width /= 2;*/
    zero_waste_apparels::solution(items, width);
}

void zero_waste_apparels::normalize(PolygonInput &polygon)
{
    double _x = INF, _y = INF;
    for (auto &point : polygon)
    {
        _x = std::min(_x, point.first);
        _y = std::min(_y, point.second);
    }
    for (auto &point : polygon)
    {
        point.first -= _x;
        point.second -= _y;
    }
}

void zero_waste_apparels::solution(std::vector<PolygonInput> &items, double &width)
{
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

    // step 1: selection heuristic: sort by decreasing order of area
    sort(polygons.begin(), polygons.end(), [](Polygon a, Polygon b) {
        return (boost_geo::area(a) >= boost_geo::area(b));
    });

    // step 2: placement heuristic: bottom left fill
    MultiPolygon resultMultiPolygon;
    for (int i = 0; i < numberOfItems; i += 1)
    {
        zero_waste_apparels::placeItem(resultMultiPolygon, polygons[i], width);
        boost_geo_util::visualize(resultMultiPolygon);
    }

    double resultMultiPolygonArea = boost_geo::area(resultMultiPolygon);
    Box stock;
    boost_geo::envelope(resultMultiPolygon, stock);
    double stockArea = boost_geo::area(stock);
    Point stockDimension = stock.max_corner();
    boost_geo::subtract_point(stockDimension, stock.min_corner());

    std::cout << "total area of items........: " << totalAreaOfItems << std::endl;
    std::cout << "result polygon set area....: " << resultMultiPolygonArea << std::endl;
    std::cout << "stock dimension [l * w]....: "
              << "[" << std::fabs(stockDimension.x) << ", " << std::fabs(stockDimension.y) << "]" << std::endl;
    std::cout << "stock area.................: " << stockArea << std::endl;
    std::cout << "packing density............: " << (totalAreaOfItems / stockArea) * 100 << " %" << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function.....: " << duration.count() / 1000000.0 << " seconds" << std::endl;

    //std::cout << boost_geo::wkt(resultMultiPolygon) << std::endl;
    boost_geo_util::visualize(resultMultiPolygon, "final");
}

double zero_waste_apparels::getWidth(Polygon &polygon)
{
    double max_y = -INF;
    double min_y = INF;

    for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
    {
        max_y = std::max(max_y, (*it).y);
        min_y = std::min(min_y, (*it).y);
    }
    return max_y - min_y;
}

double zero_waste_apparels::getLength(Polygon &polygon)
{
    double max_x = -INF;
    double min_x = INF;

    for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
    {
        max_x = std::max(max_x, (*it).x);
        min_x = std::min(min_x, (*it).x);
    }
    return max_x - min_x;
}

void zero_waste_apparels::normalizePolygon(Polygon &polygon)
{
    double min_x = INF;
    double min_y = INF;
    for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
    {
        min_x = std::min(min_x, (*it).x);
        min_y = std::min(min_y, (*it).y);
    }
    for (auto it = boost::begin(boost_geo::exterior_ring(polygon)); it != boost::end(boost_geo::exterior_ring(polygon)); ++it)
    {
        Point t = *it;
        t = Point(t.x - min_x, t.y - min_y);
        *it = t;
    }
}

void zero_waste_apparels::placeItem(MultiPolygon &stock, Polygon &piece, double &width)
{
    //width = 58;
    MultiPolygon rotatedPieces;
    for (double r = 0; r < 360; r += 90)
    {
        Polygon rotatedPiece;
        boost_geo::transform(piece, rotatedPiece, trans::rotate_transformer<boost_geo::degree, double, 2, 2>(r));
        zero_waste_apparels::normalizePolygon(rotatedPiece);
        rotatedPieces.push_back(rotatedPiece);
    }

    Polygon optimalPiece;
    double optimalVal_x = INF, optimalVal_y = INF, optimalVal_z = INF;

    for (auto poly : rotatedPieces)
    {
        double lengthOfPiece = zero_waste_apparels::getLength(poly);
        double widthOfPiece = zero_waste_apparels::getWidth(poly);

        double length = 300, val_z = INF, val_x = INF, val_y = INF;
        Polygon _piece;
        for (double _y = 0; _y <= length - widthOfPiece; _y += 1)
        {
            for (double _x = 0; _x <= width - lengthOfPiece; _x += 1)
            {
                if (vis[(int)_x][(int)_y])
                    continue;
                Polygon translatedPiece;
                MultiPolygon translatedPieces;
                boost_geo::transform(poly, translatedPiece, trans::translate_transformer<double, 2, 2>(_x, _y));
                translatedPieces.push_back(translatedPiece);
                if (boost_geo_util::isPolygonIntersectPolygon(stock, translatedPieces) == false)
                {
                    if (val_y > _y + widthOfPiece)
                    {
                        val_y = _y + widthOfPiece;
                        val_x = _x + lengthOfPiece;
                        _piece = translatedPiece;
                    }
                    else if (val_y == _y + widthOfPiece and val_x > _x + lengthOfPiece)
                    {
                        val_y = _y + widthOfPiece;
                        val_x = _x + lengthOfPiece;
                        _piece = translatedPiece;
                    }
                }
            }
        }

        if (val_y < optimalVal_y)
        {
            optimalVal_x = val_x;
            optimalVal_y = val_y;
            optimalPiece = _piece;
        }
        else if (val_y == optimalVal_y and val_x < optimalVal_x)
        {
            optimalVal_x = val_x;
            optimalPiece = _piece;
        }
    }

    double widthOfPiece = zero_waste_apparels::getWidth(optimalPiece);
    double lengthOfPiece = zero_waste_apparels::getLength(optimalPiece);
    for (double _x = optimalVal_x; _x <= optimalVal_x + lengthOfPiece; _x += 1)
    {
        for (double _y = optimalVal_y; _y <= optimalVal_y + widthOfPiece; _y += 1)
        {
            if (vis[(int)_x][(int)_y])
                continue;
            MultiPolygon mpoly;
            mpoly.push_back(optimalPiece);
            Point t(_x, _y);
            vis[(int)_x][(int)_y] = boost_geo_util::isPointInsidePolygons(mpoly, t);
        }
    }
    stock.push_back(optimalPiece);
}