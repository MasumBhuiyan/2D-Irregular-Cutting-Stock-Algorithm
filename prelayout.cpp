#include <bits/stdc++.h>

using namespace std;

using D = double;

const D EPS = 1e-8, PI = acos(-1.0), INF = 1e20;

int compare(D x) {  return fabs(x) < EPS ? 0 : x > EPS ? 1 : -1; }
D square(D x) { return x * x; }
D cube(D x) { return x * x * x; }
D safeCeil(D x) { return floor(x * 1000 + 0.5) / 1000; }

struct point {
    D x, y;
    
    void read() { cin >> x >> y; }
    void print() { cout << "(" << x << ", " << y << ")" << '\n'; }
    
    point() { x = 0; y = 0; }
    point(D p, D q) { x = p; y = q; }

    point operator + (point p) { return point(p.x + x, p.y + y); }
    point operator - (point p) { return point(x - p.x, y - p.y); }
    point operator * (D v) { return point(x * v, y * v); }
    point operator / (D v) { return point(x / v, y / v); }
    bool operator < (point p) const { return !compare(x - p.x) ? y < p.y : x < p.x; }
    bool operator == (point p) { return (!compare(x - p.x) && !compare(y - p.y)); }

    point rotate(D degree) {
        D A = degree * PI / 180.0;
        return {x * cos(A) - y * sin(A), x * sin(A) + y * cos(A)};
    }
};

struct polygon {
    int n;
    vector<point> vertex;

    void read() {
        cin >> n; vertex.resize(n);
        for(auto &p : vertex) p.read();
    } 
    void print() {
        cout << "  points(" << n << "):\n";
        for(auto &p : vertex) { 
            cout << "   ";p.print();
        }
    }
};
 
vector<polygon> read() {
    int n; cin >> n;
    vector<polygon> polygons(n);
    for(auto& poly : polygons) poly.read();

    for(int i = 0; i < n; i += 1) {
        cout << "Polygon[" << i << "]\n";
        polygons[ i ].print(); cout << endl;
    }

    return polygons;
}
//====================================================



int main() {
    vector<polygon> p = read();
    return 0;
}