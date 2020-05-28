#include "geometry.hpp"



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



void polygon::read() {
    cin >> n; 
    v.clear(); 
    v.resize(n);
    
    for(auto &p : v) 
    	p.read();
}



void polygon::print() {
    cout << "  points(" << n << "):\n";
    for(auto &p : v) {
        cout << "   ";
        p.print();
    }
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



tuple<int, int> minRectangleEnclosure(polygon a) {
	assert(a.v.size() > 0);
	assert(a.n > 0);
	assert(a.n == a.v.size());

	double x1, y1, x2, y2; 
	x1 = y1 = 2e9;
	x2 = y2 = -2e9;

	for(auto &p : a.v) {
		x1 = min(x1, p.x); x2 = max(x2, p.x);
		y1 = min(y1, p.y); y2 = max(y2, p.y);
	}
	
	int x = (int)ceil(abs(x2 - x1));
	int y = (int)ceil(abs(y2 - y1));
	return {x, y};
}



tuple<int, int> orient(polygon &a) {
	assert(a.v.size() > 0);
	assert(a.n > 0);
	assert(a.n == a.v.size());

	vector<tuple<int, int, int, double>> vec;
	for(auto deg : rotations) {
		int x, y; tie(x, y) = minRectangleEnclosure(a.rotate(deg));
		vec.emplace_back(x * y, -x, -y, deg);
	}
	sort(vec.begin(), vec.end());
	
	int area, x, y; 
	double deg;

	tie(area, x, y, deg) = vec[ 0 ];
	a = a.rotate(deg);
	return {-x, -y};
}


vector<polygon> sort(vector<polygon> &polygons) {
	vector<tuple<int, int, int, int>> vec;
	for(int i = 0; i < polygons.size(); i += 1) {
		int x, y; tie(x, y) = orient(polygons[ i ]);
		vec.emplace_back(-(x * y), -x, -y, i);
	}
	sort(vec.begin(), vec.end());
	vector<polygon> ans;
	for(auto &t : vec) {
		int area, x, y, i;
		tie(area, x, y, i) = t;
		ans.emplace_back(polygons[ i ]); 
	}
	return ans;
} 
