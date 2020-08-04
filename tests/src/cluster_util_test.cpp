#include <cassert>
#include <cluster_util.hpp>
#include <custom_assertion.hpp>

Polygon readTestcase(std::string testcaseName)
{
	std::ifstream file("../../tests/testcases/" + testcaseName + ".txt");
	if (file.fail())
	{
		std::cerr << "file does not exists\n";
		exit(-1);
	}
	int n;
	file >> n;
	Polygon polygon;
	for (int i = 0; i < n; i += 1)
	{
		double _x, _y;
		file >> _x >> _y;
		polygon.outer().push_back(Point(_x, _y));
    }
	polygon.outer().push_back(polygon.outer()[0]);
	file.close();
    return polygon;
}
int main()
{
    return 0;
}