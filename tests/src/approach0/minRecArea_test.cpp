#include "approach0.hpp"
#include "custom_assertion.hpp"

void minRecArea_test1()
{
	vector<Item> items;
	items.push_back({4, {{0,0},{5,0},{5,5},{0,5}}});
	items.push_back({3, {{0,10},{5,10},{0,15}}});
	items.push_back({3, {{5,5},{10,5},{5,10}}});
	double area = approach0::minRecArea(items);
	std::cout << area << "\n";
	ASSERT_EQUAL(150, (int)area);
}

int main()
{
	minRecArea_test1();
	return 0;
}