#include <_2D_irregular_bin_packing.hpp>

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

    // step 1: selection heuristic: sort by decreasing order of area
    sort(polygons.begin(), polygons.end(), [](Polygon a, Polygon b) {
            double area1 = boost_geo::area(a);
            double area2 = boost_geo::area(b);
            if( area1 > area2 ) return 1;
            return 0;
    });

    // step 2: placement heuristic: bottom left fill
    MultiPolygon resultMultiPolygon;
    for (int i = 0; i < numberOfItems; i += 1)
    {
        placeItem(resultMultiPolygon, polygons[i], width);
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
    std::cout << "stock dimension [l * w]....:"
              << "[" << std::fabs(stockDimension.x) << ", " << std::fabs(stockDimension.y) << "]" << std::endl;
    std::cout << "stock area.................: " << stockArea << std::endl;
    std::cout << "packing density............: " << (totalAreaOfItems / stockArea) * 100 << " %" << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() / 1000000.0 << " seconds" << std::endl;

    //std::cout << boost_geo::wkt(resultMultiPolygon) << std::endl;
    boost_geo_util::visualize(resultMultiPolygon);
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

void zero_waste_apparels::placeItem(MultiPolygon &stock, Polygon &piece, double& width)
{
    MultiPolygon rotatedPieces;
    for(double r = 0; r < 360; r += 90)
    {
        Polygon rotatedPiece;
        boost_geo::transform(piece, rotatedPiece, trans::rotate_transformer<boost_geo::degree, double, 2, 2>(r));
        normalizePolygon(rotatedPiece);
        rotatedPieces.push_back(rotatedPiece);
    }
    
    Polygon optimalPiece;
    double optimalVal_x = INF, optimalVal_y = INF, optimalVal_z = INF;

    for(auto poly : rotatedPieces)
    {
        double lengthOfPiece = zero_waste_apparels::getLength(poly); 
        double widthOfPiece = zero_waste_apparels::getWidth(poly); 
        
        double length = 300, val_z = INF, val_x = INF, val_y = INF;
        Polygon _piece;
        for(double _y = 0; _y <= length - widthOfPiece; _y += 1)
        {
            for(double _x = 0; _x <= width - lengthOfPiece; _x += 1)
            {
                if( vis[ (int)_x ][ (int)_y ] ) continue;
                Polygon translatedPiece;
                MultiPolygon translatedPieces;
                boost_geo::transform(poly, translatedPiece, trans::translate_transformer<double, 2, 2>(_x, _y));
                translatedPieces.push_back(translatedPiece);
                if( boost_geo_util::isPolygonIntersectPolygon(stock, translatedPieces) == false ) 
                {
                    if( val_y > _y + widthOfPiece )
                    {
                        val_y = _y + widthOfPiece;
                        val_x = _x + lengthOfPiece;
                        _piece = translatedPiece;
                    }
                    else if( val_y == _y + widthOfPiece and val_x > _x + lengthOfPiece )
                    {
                        val_y = _y + widthOfPiece;
                        val_x = _x + lengthOfPiece;
                        _piece = translatedPiece;
                    }
                }   
            }
        }

        if( val_y < optimalVal_y )
        {
            optimalVal_x = val_x;
            optimalVal_y = val_y;
            optimalPiece = _piece;
        }
        else if( val_y == optimalVal_y and val_x < optimalVal_x )
        {
            optimalVal_x = val_x;
            optimalPiece = _piece;
        }
    }

    double widthOfPiece = zero_waste_apparels::getWidth(optimalPiece);
    double lengthOfPiece = zero_waste_apparels::getLength(optimalPiece);
    for(double _x = optimalVal_x; _x <= optimalVal_x + lengthOfPiece; _x += 1) {
        for(double _y = optimalVal_y; _y <= optimalVal_y + widthOfPiece; _y += 1) {
            if( vis[ (int)_x ][ (int)_y ] ) continue;
            MultiPolygon mpoly; mpoly.push_back(optimalPiece);
            Point t(_x, _y);
            vis[ (int)_x ][ (int)_y ] = boost_geo_util::isPointInsidePolygons(mpoly, t);
        }
    }
    stock.push_back(optimalPiece);
}
