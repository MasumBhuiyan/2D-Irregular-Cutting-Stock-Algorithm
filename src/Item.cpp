#include "Item.hpp"

#define MAX_X 1e9
#define MAX_Y 1e9

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

/**
 * finds the minimum x and y among the points
 * set new origin to (x, y)
 * translate all the points to (x, y)
*/
void translateToNewOrigin(Polygon &points, double &max_x, double &max_y)
{
    double x = MAX_X;
    double y = MAX_Y;
    for (auto &point : points)
    {
        x = min(x, point.x);
        y = min(y, point.y);
    }
    for (auto &point : points)
    {
        point.x -= x;
        point.y -= y;
        max_x = max(max_x, point.x);
        max_y = max(max_y, point.y);
    }
}

/**
 * finds the [row, col] for the raster
*/
pair<int, int> getRasterMatrixDimension(Polygon &polygon)
{
    double max_x = -MAX_X;
    double max_y = -MAX_Y;
    translateToNewOrigin(polygon, max_x, max_y);
    int row = ceil(max_x) + 2;
    int col = ceil(max_y) + 2;
    return {row, col};
}

/**
 * scan converts the line connecting point a, b
*/
void scanConvertLine(Matrix &matrix, Point a, Point b) 
{
    // swap a, b if a.x > b.x so that always a.x <= b.x 
    if( a.x > b.x )
    {
        swap(a, b);
    }

    //always x1 <= x2 
    int x1 = (int)(a.x + 0.5),;
    int x2 = (int)(b.x + 0.5);
    int y1 = (int)(a.y + 0.5);
    int y2 =(int)(b.y + 0.5);
    int dx = x2 - x1;
    int dy = y2 - y1;

    // m = 0 i.e. horizontal line
    if( dy == 0 ) 
    {
        for(int x = x1, y = y1; x <= x2; x += 1) 
        {
            matrix[ x ][ y ] = 1;
        }
    }
    // m = infinity i.e. vertical line
    else if( dx == 0 ) 
    {
        for(int y = y1, x = x1; y <= y2; y += 1)
        {
            matrix[ x ][ y ] = 1;
        }
    }
    // 0 < m < 1 i.e. positive slope
    else if( dy > 0 ) 
    {
        int eps = 0;
        for(int x = x1, y = y1; x <= x2; x += 1) 
        {
            matrix[ x ][ y ] = 1;
            eps += dy;
            if( (eps << 1) >= dx ) 
            {
                y += 1; 
                eps -= dx;
            }
        }
    }
    // m < 0 i.e. negative slope
    else 
    {
        int eps = 0;
        for(int x = x1, y = y1; x <= x2; x += 1) 
        {
            matrix[ x ][ y ] = 1;
            eps += dy;
            if( (eps << 1) <= -dx ) 
            {
                y -= 1; 
                eps += dx;
            }
        }
    }
}

/** returns a binary matrix of the polygon(Item) */
Matrix Item::rasterize()
{
    Polygon polygon = vertices;
    pair<int, int> dimension = getRasterMatrixDimension(polygon);

    Matrix matrix(dimension.first, dimension.second);
    for(int i = 0; i < numberOfVertices; i += 1)
    {
        scanConvertLine(matrix, polygon[ i ], polygon[ (i + 1) % numberOfVertices ]);
    }

    return matrix;
}