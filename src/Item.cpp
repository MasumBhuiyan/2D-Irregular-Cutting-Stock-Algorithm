#include "Item.hpp"
#include "rasterization.hpp"

/** auxiliary functions */
/** number of vertices should be between 3 and 1000000 */
void checkPolygonValidity(int numberOfVertices) { assert(numberOfVertices >= 3 and numberOfVertices <= 1e6); }

/** constructors */
Item::Item() 
{
    vertices.clear();
    numberOfVertices = 0;
    area = 0;
    rot = 0;
    pivotPoint = {0, 0};
}
Item::Item(int n, vector<Point> v) : numberOfVertices(n)
{
    checkPolygonValidity(numberOfVertices);
    assert(v.size() == n);
    vertices = v;
    rot = 0.0;
    //pivotPoint = findCentroid();
    pivotPoint = {0, 0};
   // area = calculateArea();
    //normalize();
}

/** 
 * operator overload 
 * sorts according to non-increasing order of area
*/
bool Item::operator<(Item i) const
{
    return (area >= i.area);
}
void Item::operator +=(Point p)
{
    for(auto& point : vertices)
    {
        point.x += p.x;
        point.y += p.y;
    }
}
/** methods */
/** read from console */
void Item::read()
{
    cin >> numberOfVertices;
    checkPolygonValidity(numberOfVertices);
    vertices.resize(numberOfVertices);
    for (auto &vi : vertices)
    {
        vi.read();
    }
   // normalize();
    //pivotPoint = findCentroid();
    pivotPoint = Point(0,0);
    //area = calculateArea();
}

/** read from file pointer */
void Item::read(ifstream &file)
{
    file >> numberOfVertices;
    checkPolygonValidity(numberOfVertices);
    vertices.resize(numberOfVertices);
    for (auto &vi : vertices)
    {
        vi.read(file);
    }
    normalize();
    //pivotPoint = findCentroid();
    pivotPoint = Point(0,0);
    //area = calculateArea();
}

/** prints the object in the console */
void Item::print()
{
    cout << "number of vertices: " << numberOfVertices << endl;
    cout << "vertices: " << endl;
    for (auto &vi : vertices)
    {
        vi.print();
    }
    cout << "pivot point: ";
    pivotPoint.print();
    cout << "area " << fixed << setprecision(5) << area << endl;
}

/** show the polygon with graphics */
void Item::visualize() {}

/** 
 * retruns the rotated polygon(Item) 
 * by angle radian with respect to the Point p 
*/
Item Item::rotate(double angle, Point p)
{
    Item rotatedItem(numberOfVertices, vertices);
    for (auto &vi : rotatedItem.vertices)
    {
        vi = vi.rotate(p, angle);
    }
    rotatedItem.pivotPoint = rotatedItem.pivotPoint.rotate(p, angle);
    //rotatedItem.normalize();
    //rotatedItem.findCentroid();
    return rotatedItem;
}

/** 
 * returns the area of the polygon 
*/
double Item::calculateArea()
{
    //if( area > 0 ) return area; 
    area = 0;
    for (int i = 0; i < numberOfVertices; i++)
    {
        area += vec::cross(vertices[i], vertices[(i + 1) % numberOfVertices]);
    }
    return fabs(area) / 2.0;
}

/** 
 * finds the centroid/bary center of the polygon(Item) 
 * returns Point
*/
Point Item::findCentroid()
{
    assert(numberOfVertices == vertices.size());
    Point ret(0, 0);
    double area = 0;
    for (int i = 1; i < numberOfVertices - 1; i++)
    {
        double tmp = vec::cross(vertices[i] - vertices[0], vertices[i + 1] - vertices[0]);
        if (geo::dcmp(tmp) == 0)
        {
            continue;
        }
        area += tmp;
        ret.x += (vertices[0].x + vertices[i].x + vertices[i + 1].x) / 3.0 * tmp;
        ret.y += (vertices[0].y + vertices[i].y + vertices[i + 1].y) / 3.0 * tmp;
    }
    if (geo::dcmp(area))
    {
        ret = ret / area;
    }
    return ret;
}

/** returns a binary matrix of the polygon(Item) */
Matrix Item::rasterize()
{
    Polygon polygon = vertices;
    pair<int, int> dimension = raster::getRasterMatrixDimension(polygon);

    Matrix matrix(dimension.first, dimension.second);
    for (int i = 0; i < numberOfVertices; i += 1)
    {
        raster::scanConvertLine(matrix, polygon[i], polygon[(i + 1) % numberOfVertices]);
    }
    std::pair<int, int> seed = raster::getSeedPoint(matrix, polygon);
    raster::regionfill(matrix, seed.first, seed.second);
    return matrix;
}
void Item::normalize()
{
    double _x = 4e18, _y = 4e18;
    for(auto point : vertices)
    {
        _x = std::min(_x, point.x);
        _y = std::min(_y, point.y);
    }
    for(auto& point : vertices)
    {
        point.x -= _x;
        point.y -= _y;
    }
}
bool Item::doesOverlap(Item &a)
{
    //check seg seg intersection
    for(int i = 0; i < numberOfVertices; i += 1)
    {
        for(int j = 0; j < a.numberOfVertices; j += 1)
        {
            Point x1 = vertices[ i ];
            Point x2 = vertices[ (i + 1) % numberOfVertices ];
            Point x3 = a.vertices[ j ];
            Point x4 = a.vertices[ (j + 1) % a.numberOfVertices ];

            if( vec::segSegIntersection(x1, x2, x3, x4) == true )
            {
                return true;
            }
        }
    }
    // check point inside polygon
    for(auto point : vertices)
    {
        if( polygonal::isPointInsidePolygon(point, a.vertices) == 1 )
        {
            return true;
        }
    }
    for(auto point : a.vertices)
    {
        if( polygonal::isPointInsidePolygon(point, vertices) == 1 )
        {
            return true;
        }
    }
    return false;
}