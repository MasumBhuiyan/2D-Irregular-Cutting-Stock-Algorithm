#include <bin_packing.hpp>

/** namespace boost_geo_util */

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

bool boost_geo_util::isPolygonIntersectPolygon(MultiPolygon &multiPolygonA, MultiPolygon &multiPolygonB)
{
    for (auto polygon_a : multiPolygonA)
    {
        for (auto polygon_b : multiPolygonB)
        {
            std::deque<Polygon> output;
            boost_geo::intersection(polygon_a, polygon_b, output);
            if (output.size() > 0)
            {
                return true;
            }
        }
    }
    return false;
}

void boost_geo_util::visualize(MultiPolygon &multipolygon, std::string datasetName)
{
    Box box;
    boost::geometry::envelope(multipolygon, box);
    std::cout << "make_envelope..............: " << boost::geometry::dsv(box) << std::endl;
    std::ostringstream name;
    name << "frame_" << std::setw(4) << std::setfill('0') << frameno++ << "_" << datasetName << ".svg";
    std::ofstream svg("../diagrams/" + name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(169,169,169);stroke:rgb(169,169,169);stroke-width:1");
    mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(0,0,0);stroke-width:1;stroke-linecap:round");
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

/** namespace bin_packing */
bool vis[5000][10000];

std::vector<PolygonInput> bin_packing::readDataset(std::string filename, std::vector<int> &quantity, double &width)
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
            bin_packing::normalize(item);
            for (int k = 0; k < quantity[i]; k += 1)
            {
                items.push_back(item);
            }
        }
    }
    file.close();
    return items;
}

void bin_packing::runDataset(std::string locationName, std::string datasetName, double length)
{
    std::cout << "\033[1;32mdataset....................: " << datasetName << "\033[0m" << std::endl;
    double width;
    std::vector<int> quantity;
    std::vector<PolygonInput> items = readDataset(locationName + datasetName + ".txt", quantity, width);

    if( datasetName == "albano" )
    {
        width /= 100.0;
        length /= 100.0;

        for(auto &item : items)
        {
            for(auto& point : item)
            {
                point.first /= 100.0;
                point.second /= 100.0;
            }
        }   
    }
    bin_packing::binPacking(items, length, width, datasetName);
}

void bin_packing::normalize(PolygonInput &polygon)
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

void bin_packing::binPacking(std::vector<PolygonInput> &items, double length, double &width, std::string datasetName)
{
    auto start = std::chrono::high_resolution_clock::now();
    int numberOfItems = items.size();
    std::cout << "\033[1;32mnumber of items............: " << numberOfItems << "\033[0m" << std::endl;
    std::vector<Polygon> polygons(numberOfItems);
    double totalAreaOfItems = 0;
    for (int i = 0; i < numberOfItems; i++)
    {
        polygons[i] = boost_geo_util::constructBGPolygon(items[i]);
        totalAreaOfItems += boost_geo::area(polygons[i]);
    }

    // step 1: selection heuristic: sort by decreasing order of area
    sort(polygons.begin(), polygons.end(), [](Polygon a, Polygon b) {
        return (std::fabs(boost_geo::area(a)) > std::fabs(boost_geo::area(b)));
    });

    // step 2: placement heuristic: bottom left fill
    MultiPolygon resultMultiPolygon;
    for (int i = 0; i < numberOfItems; i += 1)
    {
        bin_packing::placeItem(resultMultiPolygon, polygons[i], length, width);
        std::cout << "[" << std::setw(3) << (int)(((i + 1) / (1.0 * numberOfItems)) * 100) << "%] "
                  << "\033[1;33mplaced..............: " << i + 1 << " / " << numberOfItems << "\033[0m" << std::endl;
    }

    double resultMultiPolygonArea = boost_geo::area(resultMultiPolygon);
    Box stock;
    boost_geo::envelope(resultMultiPolygon, stock);
    double stockArea = boost_geo::area(stock);
    Point stockDimension = stock.max_corner();
    boost_geo::subtract_point(stockDimension, stock.min_corner());

    std::ofstream output("../io/results/" + datasetName + "logs.txt");
    
    // output in console
    std::cout << "\033[1;36mtotal area of items........: " << totalAreaOfItems << "\033[0m" << std::endl;
    output << "total area of items........: " << totalAreaOfItems << std::endl;
   
    std::cout << "\033[1;36mresult polygon set area....: " << resultMultiPolygonArea << "\033[0m" << std::endl;
    output << "result polygon set area....: " << resultMultiPolygonArea << std::endl;
    
    std::cout << "\033[1;36mstock dimension [l * w]....: "
              << "[" << std::fabs(stockDimension.x) << ", " << std::fabs(stockDimension.y) << "]" << "\033[0m" << std::endl;
    output << "stock dimension [l * w]....: "
              << "[" << std::fabs(stockDimension.x) << ", " << std::fabs(stockDimension.y) << "]" << std::endl;
    
    std::cout << "\033[1;36mstock area.................: " << stockArea << "\033[0m" << std::endl;
    output << "stock area.................: " << stockArea << std::endl;
   
    std::cout << "\033[1;32mpacking density............: " << (totalAreaOfItems / stockArea) * 100 << " %\033[0m" << std::endl;
    output << "packing density............: " << (totalAreaOfItems / stockArea) * 100 << " %" << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "\033[1;35mTime taken by function.....: " << duration.count() / 1000000.0 << " seconds\033[0m" << std::endl;
    output << "Time taken by function.....: " << duration.count() / 1000000.0 << " seconds" << std::endl;

    output.close();
    std::ofstream initialSolutionWKT("../io/initialSolutions/" + datasetName + "_initial_packing.wkt");
    initialSolutionWKT << boost_geo::wkt(resultMultiPolygon) << std::endl;
    boost_geo_util::visualize(resultMultiPolygon, datasetName);
}

double bin_packing::getWidth(Polygon &polygon)
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

double bin_packing::getLength(Polygon &polygon)
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

void bin_packing::normalizePolygon(Polygon &polygon)
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

void bin_packing::placeItem(MultiPolygon &stock, Polygon &piece, double length, double &width)
{
    MultiPolygon rotatedPieces;
    for (double r = 0; r < 360; r += 90)
    {
        Polygon rotatedPiece;
        boost_geo::transform(piece, rotatedPiece, trans::rotate_transformer<boost_geo::degree, double, 2, 2>(r));
        bin_packing::normalizePolygon(rotatedPiece);
        rotatedPieces.push_back(rotatedPiece);
    }

    Polygon optimalPiece;
    double optimalVal_x = INF, optimalVal_y = INF, optimalVal_z = INF;

    for (auto poly : rotatedPieces)
    {
        double lengthOfPiece = bin_packing::getLength(poly);
        double widthOfPiece = bin_packing::getWidth(poly);

        double val_x = INF, val_y = INF;
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

    double widthOfPiece = bin_packing::getWidth(optimalPiece);
    double lengthOfPiece = bin_packing::getLength(optimalPiece);
    for (double _x = optimalVal_x; _x <= optimalVal_x + lengthOfPiece; _x += 1)
    {
        for (double _y = optimalVal_y; _y <= optimalVal_y + widthOfPiece; _y += 1)
        {
            if (vis[(int)_x][(int)_y])
            {
                continue;
            }
            MultiPolygon mpoly;
            mpoly.push_back(optimalPiece);
            Point t(_x, _y);
            vis[(int)_x][(int)_y] = boost_geo_util::isPointInsidePolygons(mpoly, t);
        }
    }
    stock.push_back(optimalPiece);
}