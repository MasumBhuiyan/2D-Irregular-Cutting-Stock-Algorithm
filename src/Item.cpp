#include "Item.hpp"
#include "rasterization.hpp"

/** auxiliary functions */
/** number of vertices should be between 3 and 1000000 */
void checkPolygonValidity(int numberOfVertices) { assert(numberOfVertices >= 3 and numberOfVertices <= 1e6); }

/** constructors */
Item::Item() {}
Item::Item(int n, vector<Point> v) : numberOfVertices(n)
{
    checkPolygonValidity(numberOfVertices);
    assert(v.size() == n);
    vertices = v;
    pivotPoint = findCentroid();
    area = calculateArea();
}

/** 
 * operator overload 
 * sorts according to non-increasing order of area
*/
bool Item::operator<(Item i) const
{
    return (area >= i.area);
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
    pivotPoint = findCentroid();
    area = calculateArea();
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
    pivotPoint = findCentroid();
    area = calculateArea();
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
    rotatedItem.findCentroid();
    return rotatedItem;
}

/** 
 * returns the area of the polygon 
*/
double Item::calculateArea()
{
    double area = 0;
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
    Point seed = pivotPoint;
    pair<int, int> dimension = raster::getRasterMatrixDimension(polygon, seed);

    Matrix matrix(dimension.first, dimension.second);
    for (int i = 0; i < numberOfVertices; i += 1)
    {
        raster::scanConvertLine(matrix, polygon[i], polygon[(i + 1) % numberOfVertices]);
    }
    raster::regionfill(matrix, seed.x, seed.y);
    return matrix;
}