#include "point.h"

point::point() : x(0), y(0) {}
point::point(double _x, double _y) : x(_x), y(_y) {}

void point::read() { cin >> x >> y; }
void point::print() { cout << "(" << x << ", " << y << ")" << '\n'; }

double point::det(point q) { return x * q.y - y * q.x; }
point point::operator + (point p) { return point(p.x + x, p.y + y); }
point point::operator - (point p) { return point(x - p.x, y - p.y); }
point point::operator * (double v) { return point(x * v, y * v); }
point point::operator / (double v) { return point(x / v, y / v); }

point point::rotate(double degree) {
    double A = degree * acos(-1.0) / 180.0;
    return {x * cos(A) - y * sin(A), x * sin(A) + y * cos(A)};
}
