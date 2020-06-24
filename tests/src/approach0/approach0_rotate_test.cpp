#include "approach0.hpp"

void approach0_rotate_test1()
{
	vector<Item> items;
	items.push_back({4, {{0,0},{5,0},{5,5},{0,5}}});
	items.push_back({3, {{0,10},{5,10},{5,15}}});
	items.push_back({3, {{5,5},{10,5},{5,10}}});
	approach0::rotate(items, 90);
	for(auto item : items) 
	{
		item.print();
	}
}

int main()
{
	approach0_rotate_test1();
	return 0;
}