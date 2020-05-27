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

    polygon rotate(D degree) {
        polygon poly;
        poly.n = n;
        for(auto p : vertex)
            poly.vertex.push_back(p.rotate(degree));
        return poly;
    }
    D areaPolygon() {
        
    }
};

D enclosingRectangleArea(polygon poly) {
    D x1 = 4e18, x2 = -4e18, y1 = 4e18, y2 = -4e18;
    for(auto &p : poly.vertex) {
        x1 = min(x1, p.x); x2 = max(x2, p.x);
            y1 = min(y1, p.y); y2 = max(y2, p.y);
    }
    return abs(x2 - x1) * abs(y2 - y1);
}

vector<polygon> orient(vector<polygon> polygons) {
    for(auto &p : polygons) {

        vector<pair<D,D>> vec;
        vec.push_back({enclosingRectangleArea(p), 0});
        vec.push_back({enclosingRectangleArea(p.rotate(90)), 90});
        vec.push_back({enclosingRectangleArea(p.rotate(180)), 180});
        vec.push_back({enclosingRectangleArea(p.rotate(270)), 270});
        
        sort(vec.begin(), vec.end());
        p = p.rotate(vec[ 0 ].second);
    }
    return polygons;
}
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
int main() {
    vector<polygon> p = read();
    p = orient(p);
    cout << "after orientation: " << endl;
    for(int i = 0; i < p.size(); i += 1) {
        cout << "Polygon[" << i << "]\n";
        p[ i ].print(); cout << endl;
    }
    return 0;
}

/*
5
2
1 1
1 2
3
1 2 
2 4
5 3
1
1 5
1 
4 5
1
4 8
*/