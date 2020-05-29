#include "polygon.hpp"

void polygon::read()
{
    cin >> n;
    v.clear();
    v.resize(n);

    for (auto &p : v)
        p.read();
}

void polygon::print()
{
    cout << "  points(" << n << "):\n";
    for (auto &p : v)
    {
        cout << "   ";
        p.print();
    }
}

double polygon::area()
{
    double ans = 0;
    for (int i = 0; i < n; i += 1)
        ans += v[i].det(v[(i + 1) % n]);
    return fabs(ans) / 2.0;
}

polygon polygon::rotate(double degree)
{
    polygon poly;
    poly.n = n;
    for (auto p : v)
        poly.v.push_back(p.rotate(degree));
    return poly;
}
