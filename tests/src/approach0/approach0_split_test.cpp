#include <iostream>
#include <iomanip>
#include <Item.hpp>
#include <geometry.hpp>
#include <vector>
#include <string>
#include <approach0.hpp>
#include <algorithm>
#include <assert.h>
#include<map>

int n;
double width;
std::vector<int> quantity;
std::vector<Item> items;

void readIO(std::string filename) 
{
    std::cout << "Opening file: " << filename << " .....\n";
   
    std::ifstream file(filename); 
    if (file.fail()) 
    {
        std::cerr << "File does not exists\n";
    } else 
    {
	    std::cout << "Reading file...\n";
	    file >>  width;
	    std::cout << "STOCK WIDTH: " << width << "\n";

	    file >> n;
	    std::cout << "TOTAL TYPES OF PIECES: " << n << "\n";

	    quantity.resize(n);
	    for (int i = 0; i < n; i += 1)
	    {
	    	Item item;
	    	file >> quantity[ i ];
	        item.read(file);
	        for(int k = 0; k < quantity[ i ]; k += 1)
	        	items.push_back(item);
	        std::cout << "PIECE " << i + 1 << "\n";
	        std::cout << "QUANTITY " << quantity[ i ] << "\n";
	        items.back().print();
	    }
	    std::cout << "Reading complete\n";
	    file.close();
    }
}
void getPos(std::vector<Item> items)
{
	std::vector<Item> items2 = items;
	std::map<double,int> pos;
	int x = 0;
	sort(items.begin(), items.end());
	for(auto item : items)
	{
		if( pos.find(item.area) == pos.end() ) 
			pos[ item.area ] = x++;
	}
	for(auto item : items2)
	{
		std::cout << pos[ item.area ] << " ";
	}
	std::cout << '\n';
}
int main()
{
	//readIO("../../tests/dataset/sample.txt");
	readIO("../../tests/dataset/albano.txt");
	//readIO("../../tests/dataset/blaz.txt");
	for(auto& item : items)
	{
		item.normalize();
		item.area = item.calculateArea();
		//item.print();
	}
	//sort(items.begin(), items.end());
	//random_shuffle(items.begin(), items.end());
	vector<Item> stock = approach0::split(items, 0, items.size() - 1);

	double area1 = 0.0, area2 = approach0::minRecArea(stock);
	//std::cout << "\n\n\nAfter split::\n";
	for(auto item : stock) 
	{
		area1 += item.calculateArea();
		//item.print();
	}
	std::cout << area1 << " " << area2 << '\n';
	std::cout << "Packing Density:: " << (area1 / area2) * 100 << " %\n";
	return 0;
}