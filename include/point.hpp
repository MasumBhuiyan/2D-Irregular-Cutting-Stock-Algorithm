#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

class point
{
private:
    /* data */
public:
    double x, y;

    point();
    point(double, double);
    void read();
    void print();
    double det(point);
    point operator+(point);
    point operator-(point);
    point operator*(double);
    point operator/(double);
    point rotate(double degree);
};

#endif // POINT_HPP