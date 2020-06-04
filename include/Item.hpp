#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include "Point.hpp"
#include "Matrix.hpp"
#include "geometry.hpp"

using namespace std;

class Item
{
private:
public:
    int numberOfVertices;
    vector<Point> vertices;
    Point pivotPoint;

    Item();
    Item(int, vector<Point>);

    // methods
    void read();
    void read(ifstream &);      // read from file pointer
    void print();               // prints the object in the console
    void visualize();           // show the polygon with graphics
    Item rotate(double, Point); // retruns the rotated polygon(Item) with respect to the Point
    Point findCentroid();       // finds the centroid of the polygon(Item)
    Matrix rasterize();         // returns a binary matrix of the polygon(Item)
};

#endif // ITEM_HPP