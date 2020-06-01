#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

class Point
{
private:
    /* data */
public:
    double x, y;

    Point();
    Point(double, double);
    void read();
    void print();
    double determinant(Point);
    Point operator+(Point);
    Point operator-(Point);
    Point operator*(double);
    Point operator/(double);
    Point rotate(Point, double);
};

#endif // POINT_HPP