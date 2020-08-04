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
	Polygon polygon1({{
        {-12.0, 2.0},
        {-8.0, 9.0},
        {-4.0, 4.0},
        {-12.0, 2.0}
    }});
    Polygon polygon2({{
        {-4.0, 10.0},
        {3.0, 6.0},
        {-2.0, 2.0},
        {-4.0, 10.0}
    }});
	auto convexhull = cluster_util::convexHull({polygon1, polygon2});
	geo_util::visualize({polygon1, polygon2, convexhull}, "../../tests/testResults/", "convexHull_test1");
}
void findConvexHullVacancy_test() 
{
	Polygon concavePolygon1 = readTestcase("concavepolygon1");
	geo_util::visualize({concavePolygon1}, "../../tests/testResults/", "findConvexHull_test1_img1");
	std::vector<Polygon> vacancies = cluster_util::findConvexHullVacancy(concavePolygon1);
	MultiPolygon polygons;
	for(auto vacancy: vacancies)
	{
		polygons.push_back(vacancy);
	}
	geo_util::visualize(polygons, "../../tests/testResults/", "findConvexHull_test1_img2");

	Polygon concavePolygon2 = readTestcase("concavepolygon2");
	geo_util::visualize({concavePolygon2}, "../../tests/testResults/", "findConvexHull_test2_img1");
	vacancies = cluster_util::findConvexHullVacancy(concavePolygon2);
	polygons.clear();
	for(auto vacancy: vacancies)
	{
		polygons.push_back(vacancy);
	}
	geo_util::visualize(polygons, "../../tests/testResults/", "findConvexHull_test2_img2");
}
void findOppositeSideOfVacancies_test() 
{
	Polygon concavePolygon1 = readTestcase("concavepolygon1");
	Polygon convexhull = cluster_util::convexHull({concavePolygon1});
	std::vector<Polygon> vacancies = cluster_util::findConvexHullVacancy(concavePolygon1);
	std::vector<std::vector<Point>> oppositeSideOfVacancies = cluster_util::findOppositeSideOfVacancies(convexhull, vacancies);
	for(auto line: oppositeSideOfVacancies)
	{
		std::cout << "(" << line[0].x << ", " << line[0].y << ") - ( " << line[1].x << ", " << line[1].y << ")\n"; 
	}
}
void findDominantPoint_test() 
{
	Polygon concavePolygon1 = readTestcase("concavepolygon1");
	Point dominant = cluster_util::findDominantPoint(concavePolygon1);
	std::cout << boost_geo::wkt(dominant) << "\n";
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
	std::ifstream file("../../tests/testcases/perfectClustering_test.txt");
	if (file.fail())
	{
		std::cerr << "file does not exists\n";
		exit(-1);
	}
	int tc; file >> tc;

	while( tc-- )
	{
		int n, m;
		double expectedBestClusterizationValues;
		file >> n >> m;
		std::vector<std::vector<double>> clusterValues(n, std::vector<double>(n, 0.0));
	
		for (int i = 0; i < n; i += 1)
		{
			for(int j = 0; j < n; j += 1)
			{
				file >> clusterValues[ i ][ j ];
			}
		}
		std::vector<std::tuple<int,int>> expectedClusterIds;
		file >> expectedBestClusterizationValues;
		for(int i = 0; i < m; i += 1)
		{
			double _x, _y;
			file >> _x >> _y;
			expectedClusterIds.push_back({_x, _y});
		}

		std::vector<std::tuple<int,int>> clusterIds;
		double bestClusterizationValue;
		std::tie(bestClusterizationValue, clusterIds) = cluster_util::perfectClustering(clusterValues, m);
		assert( bestClusterizationValue == expectedBestClusterizationValues
		and clusterIds == expectedClusterIds ); 
	}
	file.close();
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