#ifndef POINT_H
#define POINT_H

#include <bits/stdc++.h>

using namespace std;

struct point {
    double x, y;

    point();
    point(double, double);
    void read();
    void print();
    double det(point);
    point operator + (point);
    point operator - (point);
    point operator * (double);
    point operator / (double);
    point rotate(double degree);
};
#endif // POINT_H
