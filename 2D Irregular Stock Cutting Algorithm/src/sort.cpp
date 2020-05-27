#include "sort.h"

double enclosingRectangleArea(polygon poly) {
    double x1 = 4e18, x2 = -4e18, y1 = 4e18, y2 = -4e18;
    for(auto &p : poly.v) {
        x1 = min(x1, p.x); x2 = max(x2, p.x);
        y1 = min(y1, p.y); y2 = max(y2, p.y);
    }
    return abs(x2 - x1) * abs(y2 - y1);
}

vector<polygon> orient(vector<polygon> polygons) {
    for(auto &p : polygons) {
        vector<pair<double,double>> vec;
        vec.push_back({enclosingRectangleArea(p), 0});
        vec.push_back({enclosingRectangleArea(p.rotate(90)), 90});
        vec.push_back({enclosingRectangleArea(p.rotate(180)), 180});
        vec.push_back({enclosingRectangleArea(p.rotate(270)), 270});

        sort(vec.begin(), vec.end());
        p = p.rotate(vec[ 0 ].second);
    }
    return polygons;
}

bool polygon_comapare_function(polygon a, polygon b) {
    double a1 = a.area(), b1 = b.area();
    if( a1 > b1 ) return 1;
    if( a1 == b1) return a.len() > b.len();
    return 0;
}
void sort(vector<polygon> &polygons) {

}
