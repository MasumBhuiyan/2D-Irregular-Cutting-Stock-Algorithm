#include "geometry.hpp"

tuple<int, int> geo::minRectangleEnclosure(polygon a)
{
	assert(a.v.size() > 0);
	assert(a.n > 0);
	assert(a.n == a.v.size());

	double x1, y1, x2, y2;
	x1 = y1 = 2e9;
	x2 = y2 = -2e9;

	for (auto &p : a.v)
	{
		x1 = min(x1, p.x);
		x2 = max(x2, p.x);
		y1 = min(y1, p.y);
		y2 = max(y2, p.y);
	}

	int x = (int)ceil(abs(x2 - x1));
	int y = (int)ceil(abs(y2 - y1));
	return {x, y};
}

tuple<int, int> geo::orient(polygon &a)
{
	assert(a.v.size() > 0);
	assert(a.n > 0);
	assert(a.n == a.v.size());

	vector<double> rotations({0, 90, 180, 270});
	vector<tuple<int, int, int, double>> vec;
	for (auto deg : rotations)
	{
		int x, y;
		tie(x, y) = minRectangleEnclosure(a.rotate(deg));
		vec.emplace_back(x * y, -x, -y, deg);
	}
	sort(vec.begin(), vec.end());

	int area, x, y;
	double deg;

	tie(area, x, y, deg) = vec[0];
	a = a.rotate(deg);
	return {-x, -y};
}

vector<polygon> geo::sort(vector<polygon> &polygons)
{
	vector<tuple<int, int, int, int>> vec;
	for (int i = 0; i < polygons.size(); i += 1)
	{
		int x, y;
		tie(x, y) = orient(polygons[i]);
		vec.emplace_back(-(x * y), -x, -y, i);
	}
	sort(vec.begin(), vec.end());
	vector<polygon> ans;
	for (auto &t : vec)
	{
		int area, x, y, i;
		tie(area, x, y, i) = t;
		ans.emplace_back(polygons[i]);
	}
	return ans;
}
