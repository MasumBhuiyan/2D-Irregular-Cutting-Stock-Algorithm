#include "polygon.h"

void polygon::read() {
    cin >> n; v.resize(n);
    for(auto &p : v) p.read();
}
void polygon::print() {
    cout << "  points(" << n << "):\n";
    for(auto &p : v) {
        cout << "   ";p.print();
    }
}
double polygon::len() {
    double x1 = 4e18, x2 = -4e18;
    for(auto p : v) {
        x1 = min(x1, p.x); x2 = max(x2, p.x);
    }
    return abs(x2 - x1);
}
double polygon::area() {
    double ans = 0;
    for(int i = 0; i < n; i += 1)
        ans += v[ i ].det(v[ (i + 1) % n ]);
    return fabs(ans) / 2.0;
}
polygon polygon::rotate(double degree) {
    polygon poly;
    poly.n = n;
    for(auto p : v)
        poly.v.push_back(p.rotate(degree));
    return poly;
}

