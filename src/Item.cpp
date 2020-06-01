#include "Item.hpp"

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

/** finds the centroid of the polygon(Item) */
Point Item::findCentroid() { return Point(0, 0); }

/** returns a binary matrix of the polygon(Item) */
Matrix Item::rasterize() { return Matrix(2, 2); }