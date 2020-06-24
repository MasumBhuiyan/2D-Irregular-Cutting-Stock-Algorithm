#include "approach0.hpp"
#include "custom_assertion.hpp"

void doesOverlap_test1()
{
	//touches
	vector<Item> items1, items2;
	items1.push_back({4, {{0,0},{5,0},{5,5},{0,5}}});
	items1.push_back({3, {{0,10},{5,10},{5,15}}});
	items2.push_back({3, {{5,5},{10,5},{5,10}}});
	bool ans = approach0::doesOverlap(items1, items2);
	ASSERT_EQUAL(ans, true);
}
void doesOverlap_test2()
{
	// intersects
	vector<Item> items1, items2;
	items1.push_back({4, {{0,0},{5,0},{5,5},{0,5}}});
	items1.push_back({3, {{0,10},{5,10},{5,15}}});
	items2.push_back({3, {{4,4},{10,5},{5,10}}});
	bool ans = approach0::doesOverlap(items1, items2);
	ASSERT_EQUAL(ans, true);
}
void doesOverlap_test3()
{
	//completely inside
	vector<Item> items1, items2;
	items1.push_back({4, {{0,0},{5,0},{5,5},{0,5}}});
	items2.push_back({4, {{1,1},{2,1},{2,2},{1,2}}});
	bool ans = approach0::doesOverlap(items1, items2);
	ASSERT_EQUAL(ans, true);
}
void doesOverlap_test4()
{
	vector<Item> items1, items2;
	items1.push_back({4, {{0,0},{5,0},{5,5},{0,5}}});
	items2.push_back({3, {{0.00, 6.00},{5.00, 6.00},{5.00, 11.00}}});
	bool ans = approach0::doesOverlap(items1, items2);
	ASSERT_EQUAL(ans, false);
}
int main()
{
	doesOverlap_test1();
	doesOverlap_test2();
	doesOverlap_test3();
	doesOverlap_test4();
	return 0;
}