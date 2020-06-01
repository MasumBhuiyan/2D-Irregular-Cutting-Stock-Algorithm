#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

class Point
{
private:
    /* data */
    double x, y;

public:
    Point();
    Point(double, double);

    // getter & setter
    double getX();
    void setX(double);

    double getY();
    void setY(double);

    // methods
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