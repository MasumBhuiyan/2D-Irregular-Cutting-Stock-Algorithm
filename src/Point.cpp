#include "Point.hpp"
#include "geometry.hpp"

/** constructors */
Point::Point() {}
Point::Point(double _x, double _y) : x(_x), y(_y) {}

/** operator overloading */
bool Point::operator==(const Point &u) const { return geo::dcmp(x - u.x) == 0 && geo::dcmp(y - u.y) == 0; }
bool Point::operator!=(const Point &u) const { return !(*this == u); }
bool Point::operator<(const Point &u) const { return geo::dcmp(x - u.x) < 0 || (geo::dcmp(x - u.x) == 0 && geo::dcmp(y - u.y) < 0); }
bool Point::operator>(const Point &u) const { return u < *this; }
bool Point::operator<=(const Point &u) const { return *this < u || *this == u; }
bool Point::operator>=(const Point &u) const { return *this > u || *this == u; }
Point Point::operator+(const Point &u) { return Point(x + u.x, y + u.y); }
Point Point::operator-(const Point &u) { return Point(x - u.x, y - u.y); }
Point Point::operator*(const double u) { return Point(x * u, y * u); }
Point Point::operator/(const double u) { return Point(x / u, y / u); }
double Point::operator*(const Point &u) { return x * u.y - y * u.x; }

/** methods */
void Point::read() { std::cin >> x >> y; }
void Point::read(std::ifstream &file) { file >> x >> y; }
void Point::print() { std::cout << std::setprecision(2) << std::fixed << "(" << x << ", " << y << ")" << std::endl; }

double Point::determinant(Point q) { return x * q.y - y * q.x; }

/**
 * rotate by angle with respect to p  
*/
Point Point::rotate(Point p, double angle)
{
    Point v = Point(x, y) - p;
    double c = cos(angle), s = sin(angle);
    return {p.x + v.x * c - v.y * s, p.y + v.x * s + v.y * c};
}