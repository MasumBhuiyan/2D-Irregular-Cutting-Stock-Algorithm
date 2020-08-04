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

void convexHull_test() 
{
	MultiPolygon polygon;
	auto polygonHull = cluster_util::convexHull(polygon);
}
void findConvexHullVacancy_test() 
{

}
void findOppositeSideOfVacancies_test() 
{

}
void findDominantPoint_test() 
{

}
void findAllPairDominantPoint_test() 
{

}
void findAllConvexHullVacancies_test() 
{

}
void clusteringCriteria1_test() 
{

}
void clusteringCriteria2_test() 
{

}
void getClusterValue_test() 
{

}
void sort_test() 
{

}
void getCandidatePlacementPositions_test() 
{

}
void findBlfPoint_test() 
{

}
void blf_test() 
{

}
void getBestClusters_test() 
{

}
void printBestClusters_test() 
{

}
void perfectClustering_test() 
{

}
void getClusterValues_test() 
{

}
void generateInitialSolution_test() 
{

}

int main()
{
	convexHull_test();
	findConvexHullVacancy_test();
	findOppositeSideOfVacancies_test();
	findDominantPoint_test();
	findAllPairDominantPoint_test();
	findAllConvexHullVacancies_test();
	clusteringCriteria1_test();
	clusteringCriteria2_test();
	getClusterValue_test();
	sort_test();
	getCandidatePlacementPositions_test();
	findBlfPoint_test();
	blf_test();
	getBestClusters_test();
	printBestClusters_test();
	perfectClustering_test();
	getClusterValues_test();
	generateInitialSolution_test();
    return 0;
}