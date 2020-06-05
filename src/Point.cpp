#include "Point.hpp"

/** constructors */
Point::Point() {}
Point::Point(double _x, double _y) : x(_x), y(_y) {}

/** methods */
void Point::read() { std::cin >> x >> y; }
void Point::read(std::ifstream &file) { file >> x >> y; }
void Point::print() { std::cout << std::setprecision(2) << std::fixed << "(" << x << ", " << y << ")" << std::endl; }

double Point::determinant(Point q) { return x * q.y - y * q.x; }
Point Point::operator+(Point p) { return Point(p.x + x, p.y + y); }
Point Point::operator-(Point p) { return Point(x - p.x, y - p.y); }
Point Point::operator*(double v) { return Point(x * v, y * v); }
Point Point::operator/(double v) { return Point(x / v, y / v); }
Point Point::rotate(Point p, double angle)
{
    Point v = Point(x, y) - p;
    double c = cos(angle), s = sin(angle);
    return {p.x + v.x * c - v.y * s, p.y + v.x * s + v.y * c};
}