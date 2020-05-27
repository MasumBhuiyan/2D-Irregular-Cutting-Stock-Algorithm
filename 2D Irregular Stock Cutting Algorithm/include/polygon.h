#ifndef POLYGON_H
#define POLYGON_H

#include <bits/stdc++.h>
#include "point.h"

using namespace std;

struct polygon {
    int n;
    vector<point> v;
    void read();
    void print();
    double len();
    double area();
    polygon rotate(double);
};
#endif // POLYGON_H
