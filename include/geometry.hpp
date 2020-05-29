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


struct polygon {
    int n;
    vector<point> v;

    void read();
    void print();
    double area();
    polygon rotate(double);
};


tuple<int, int> minRectangleEnclosure(polygon);
tuple<int, int> orient(polygon&);
vector<polygon> sort(vector<polygon>&); 
