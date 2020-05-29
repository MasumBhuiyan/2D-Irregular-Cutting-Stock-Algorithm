#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <iostream>
#include <vector>

#include "point.hpp"

using namespace std;

class polygon
{
private:
    /* data */
public:
    int n;
    vector<point> v;

    void read();
    void print();
    double area();
    polygon rotate(double);
};

#endif // POLYGON_HPP