#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

class Point
{
private:
public:
    double x, y;

    Point();
    Point(double, double);

    // operator overloading
    bool operator==(const Point &u) const;
    bool operator!=(const Point &u) const;
    bool operator<(const Point &u) const;
    bool operator>(const Point &u) const;
    bool operator<=(const Point &u) const;
    bool operator>=(const Point &u) const;
    Point operator+(const Point &u);
    Point operator-(const Point &u);
    Point operator*(const double u);
    Point operator/(const double u);
    double operator*(const Point &u);

    // methods
    void read();
    void read(std::ifstream &);
    void print();
    double determinant(Point);
    Point rotate(Point, double);
};

#endif // POINT_HPP