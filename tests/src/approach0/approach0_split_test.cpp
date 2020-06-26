#include <iostream>
#include <iomanip>
#include <Item.hpp>
#include <geometry.hpp>
#include <vector>
#include <string>
#include <approach0.hpp>
#include <algorithm>
#include <chrono> 
#include <assert.h>
#include<map>
using namespace std::chrono; 
int n;
double width;
std::vector<int> quantity;
std::vector<Item> items;

void readIO(std::string filename) 
{
    std::ifstream file(filename); 
    if (file.fail()) 
    {
        std::cerr << "File does not exists\n";
    } else 
    {
	    file >>  width;
	    file >> n;
	    quantity.resize(n);
	    for (int i = 0; i < n; i += 1)
	    {
	    	Item item;
	    	file >> quantity[ i ];
	        item.read(file);
	        for(int k = 0; k < quantity[ i ]; k += 1)
	        	items.push_back(item);
	    }
	    file.close();
    }
}
void processVisualize(std::vector<Item> stock)
{
	double _x = 4e18, _y = 4e18;
	for(auto &item : stock)
	{
		for(auto &point : item.vertices)
		{
			_x = std::min(_x, point.x);
			_y = std::min(_y, point.y);
		}
	}
	for(auto &item : stock)
	{
		for(auto &point : item.vertices)
		{
			point.x -= _x;
			point.y -= _y;
		}
	}
	_x = -4e18, _y = -4e18;
	for(auto &item : stock)
	{
		for(auto &point : item.vertices)
		{
			_x = std::max(_x, point.x);
			_y = std::max(_y, point.y);
		}
	}

	_x = 800 / _x; 
	_y = 600 / _y;


	for(auto &item : stock)
	{
		for(auto &point : item.vertices)
		{
			point.x *= _x;
			point.y *= _y;
		}
	}

	std::cout << "[";
	for(int i = 0; i < stock.size(); i += 1)
	{
		Item item = stock[ i ];
		std::cout <<"[" << item.vertices[0].x << ", " << item.vertices[0].y;
		for(int j = 1; j < item.vertices.size(); j += 1)
		{
			Point point = item.vertices[ j ];
			std::cout << std::setprecision(2) << std::fixed << ", " << point.x<< ", " << point.y;
		}
		std::cout << std::setprecision(2) << std::fixed << ", " << item.vertices[0].x << ", " << item.vertices[0].y << "]\n";
		if( i < stock.size() - 1 ) std::cout << ",";
	}
	std::cout << "]\n";
}
int main()
{
	auto start = high_resolution_clock::now(); 

	readIO("../../tests/dataset/sample.txt");
	//readIO("../../tests/dataset/albano.txt");
	//readIO("../../tests/dataset/blaz.txt");
	//readIO("../../tests/dataset/shirts.txt"); //items size 99
	//readIO("../../tests/dataset/trousers.txt"); // items size 64
	//readIO("../../tests/dataset/marques.txt");
	//readIO("../../tests/dataset/dagli.txt");
	//readIO("../../tests/dataset/jakobs1.txt");
	//readIO("../../tests/dataset/jakobs2.txt");
	for(auto& item : items)
	{
		item.normalize();
		item.area = item.calculateArea();
	}
	
	vector<Item> stock = approach0::split(items, 0, items.size() - 1);
	for(int i = 0; i < stock.size(); i += 1) {
		for(int j = i + 1; j < stock.size(); j += 1) {
			assert(stock[ i ].doesOverlap(stock[ j ]) == false);
		}
	}

	processVisualize(stock);

	double area1 = 0.0, area2 = approach0::minRecArea(stock);
	for(auto item : stock) 
	{
		area1 += item.calculateArea();
	}
	std::cout << "Packing Density:: " << (area1 / area2) * 100 << " %\n";
	auto stop = high_resolution_clock::now(); 
	auto duration = duration_cast<microseconds>(stop - start); 
    std::cout << "Time taken by function: " << duration.count() / 1000000.0 << " seconds" << std::endl; 
	return 0;
}