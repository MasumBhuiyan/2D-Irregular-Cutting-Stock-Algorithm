#include <approach0.hpp>
#include <array>

double _dx[8] = {0, -0.001, -0.001, -0.001, 0, 0.001, 0.001, 0.001};
double _dy[8] = {-0.001, -0.001, 0, 0.001, 0.001, 0.001, 0, -0.001};

double approach0::minRecArea(std::vector<Item> &items)
{
	double _x = 4e18, _y = 4e18;
	for(auto item : items)
	{
		for(auto point : item.vertices)
		{
			_x = std::min(_x, point.x);
			_y = std::min(_y, point.y);
		}
	}
	double max_x = -4e18, max_y = -4e18;
	for(auto item : items)
	{
		for(auto point : item.vertices)
		{
			max_x = std::max(max_x, point.x - _x);
			max_y = std::max(max_y, point.y - _y);
		}
	}
	//std::cout << "max_x " << max_x << "\n";
	return max_x * max_y;
}

void approach0::rotate(std::vector<Item> &items, double angle)
{
	double _x = 4e18, _y = 4e18;
	if( fabs(angle) < geo::EPS ) return ;
	for(auto& item : items)
	{
		for(auto &point : item.vertices)
		{
			point = point.rotate(Point(0,0),geo::DEG2RAD(angle));
			_x = std::min(_x, point.x);
			_y = std::min(_y, point.y);
		}
	}
	
	for(auto& item : items)
	{
		for(auto &point : item.vertices)
		{
			point.x -= _x;
			point.y -= _y;
		}
	}
}
bool approach0::doesOverlap(std::vector<Item>&A, std::vector<Item>&B)
{
	for(auto &item1 : A) 
	{
		for(auto &item2 : B)
		{
			if( item1.doesOverlap(item2) ) return true;
		}
	}
	return false;
}
std::tuple<double, vector<Item>> approach0::merge(std::vector<Item> &A, std::vector<Item> &B)
{
	std::set<Point> pointSet;
	for(auto& item : A)
	{
		for(auto& point : item.vertices)
		{
			for(int k = 0; k < 8; k += 1)
			{
				double _x = point.x + _dx[ k ];
				double _y = point.y + _dy[ k ];
				pointSet.insert(Point(_x, _y));
				
			}
		}
	}
	Point pivotPoint(0,0);
	std::vector<Item> rec;
	double minArea = 4e18;
	for(auto point : pointSet)
	{
		std::vector<Item> C = B;
		for(auto& item : C)
			item += point;

		//C.back().print();
		if( doesOverlap(A, C) == true ) continue;
		
		std::vector<Item> D = A;
		for(auto &item : C)
		{
			D.push_back(item);
		}
		double area = minRecArea(D);
		if( area < minArea )
		{
			minArea = area;
			rec = D;
		}
	}
	//std::cout << "*** " << minArea << std::endl;
	return {minArea, rec};
}
std::vector<Item> approach0::split(std::vector<Item> &items, int l, int r)
{
	if( l == r ) return {items[ l ]};
	int m = (l + r) >> 1;
	std::vector<Item> a = split(items, l, m);
	std::vector<Item> b = split(items, m + 1, r);

	double angle = 90;
	double area = 4e18;
	std::vector<Item> c;
	for(int i = 0; i < (360/(int)angle); i += 1) 
	{
		for(int j = 0; j < (360/(int)angle); j += 1)
		{
			std::vector<Item> _c;
			double _area;
			tie(_area, _c) = merge(a, b);
			if( _area < area )
			{
				area = _area;
				c = _c;
			}
			rotate(b, angle);
		}
	    rotate(a, angle);
	}
	return c;
}