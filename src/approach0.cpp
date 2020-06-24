#include <approach0.hpp>

std::tuple<double, vector<Item>> approach0::merge(std::vector<Item> &a, std::vector<Item> &b)
{
	vector<Item> mergedItems;
	double area;

	std::set<Point> pointSet;
	for(auto& item : a)
	{
		for(auto& point : item.vertices)
		{
			pointSet.insert(point);
		}
	}

	Point piv;
	for(auto point : pointSet)
	{
		vector<Item> c = b;
		// translate b to point 
		for(auto& item : c)
		{
			for(auto &_point : item.vertices)
			{
				_point.x += point.x;
				_point.y += point.y;
			}
		}
		// check overlaps
		int flag = 1;
		for(auto item1 : c)
		{
			for(auto item2 : a)
			{
				if( item1.doesOverlap(item2) )
				{
					flag = 0;
					break;
				}
			}
			if( !flag ) break;
		}
		if( flag )
		{
			piv = point;
			break;
		}
	}
	double _x = 4e18, _y = 4e18;
	for(auto item : a)
	{
		for(auto point : item.vertices)
		{
			_x = std::min(_x, point.x);
			_y = std::min(_y, point.y);
		}
	}
	for(auto item : b)
	{
		for(auto point : item.vertices)
		{
			_x = std::min(_x, piv.x + point.x);
			_y = std::min(_y, piv.y + point.y);
		}
	}
	double max_x = -4e18, max_y = -4e18;
	for(auto item : a)
	{
		for(auto point : item.vertices)
		{
			max_x = std::max(max_x, point.x - _x);
			max_y = std::max(max_y, point.y - _y);
		}
	}
	for(auto item : b)
	{
		for(auto point : item.vertices)
		{
			max_x = std::max(max_x, point.x - _x);
			max_y = std::max(max_y, point.y - _y);
		}
	}
	area = max_x * max_y;
	return {area, mergedItems};
}
void approach0::rotate(std::vector<Item> &items, double angle)
{
	for(auto& item : items)
	{
		item = item.rotate(geo::DEG2RAD(angle), Point(0,0));
	}
}	
std::vector<Item> approach0::split(std::vector<Item> &items, int l, int r)
{
	if( l == r )
	{
		return {items[ l ]};
	}

	int m = (l + r) >> 1;
	std::vector<Item> a = split(items, l, m);
	std::vector<Item> b = split(items, m + 1, r);

	std::vector<Item> c;
	double minArea = 4e18, angle = 90;
	int rot = 360 / (int)angle;

	for(int i = 0; i < rot; i += 1)
	{
		rotate(a, angle);
		for(int j = 0; j < rot; j += 1) 
		{
			rotate(b, angle);
			std::vector<Item> d;
			double area;
			std::tie(area, d) = merge(a, b);
			if( area < minArea ) 
			{
				c = d;
				minArea = area;
			}
		}
	}
	return c;
}