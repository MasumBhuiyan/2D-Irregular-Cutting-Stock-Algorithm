#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <vector>

#include "Point.hpp"
#include "Matrix.hpp"

using namespace std;

class Item
{
private:
public:
    int numberOfVertices;
    vector<Point> vertices;
    Point pivotPoint;

    Item(/* args */);
    ~Item();

    // methods
    void read();
    void print();               // prints the object in the console
    void visualize();           // show the polygon with graphics
    Item rotate(double, Point); // retruns the rotated polygon(Item) with respect to the Point
    Point calculateCentroid();  // finds the centroid of the polygon(Item)
    Matrix rasterize();         // returns a binary matrix of the polygon(Item)
};

#endif // ITEM_HPP