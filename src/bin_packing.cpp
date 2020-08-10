#include <libnfporb.hpp>
#include <bin_packing.hpp>

/** namespace cluster_util */

/** namespace bin_packing */

/** namespace bin_packing */
/** 
 * read input from dataset file
 * file location ../io/datasets/**.txt
 */
std::tuple<vector<Polygon>, long double> bin_packing::readDataset(std::string datasetName)
{
	long double width;
	vector<Polygon> polygons;
	std::ifstream file("../io/datasets/" + datasetName + ".txt");
	if (file.fail())
	{
		std::cerr << "file does not exists\n";
		exit(-1);
	}
	else
	{
		int n;
		file >> width;
		file >> n;
		for (int i = 0; i < n; i += 1)
		{
			int quantity;
			file >> quantity;
			int numberOfVirtices;
			Polygon polygon;
			file >> numberOfVirtices;
			for (int j = 0; j < numberOfVirtices; j++)
			{
				long double _x, _y;
				file >> _x >> _y;
				polygon.outer().push_back(Point(_x, _y));
			}
			polygon.outer().push_back(polygon.outer()[0]);
			boost_geo::correct(polygon);
			while (quantity--)
			{
				polygons.push_back(polygon);
			}
		}
	}
	std::cout << "Input polygons\n";
	for (auto p : polygons)
	{
		std::cout << boost_geo::wkt(p) << '\n';
	}
	file.close();
	return {polygons, width};
}

/** bin packing */
void bin_packing::binPacking(
	vector<Polygon> &polygons, long double width, string outputLocation, string datasetName, long double runTimeDuration)
{
	// create directory for the dataset
	string outputDirectoryName = outputLocation + "/" + datasetName;
	mkdir(outputDirectoryName.c_str(), 0777);

	auto start = std::chrono::high_resolution_clock::now();

	// create directory to store wkt files of input polygons
	string inputPolygonsWKTStorageLocation = outputDirectoryName + "/input_polygons_wkt_files";
	mkdir(inputPolygonsWKTStorageLocation.c_str(), 0777);

	double totalAreaOfInputPolygons = 0;
	for (int i = 0; i < polygons.size(); i++)
	{
		Polygon polygon = polygons[i];
		// write the wkt polygon file
		std::ostringstream polygonWKTFileName;
		polygonWKTFileName << "polygon_" << std::setw(2) << std::setfill('0') << i << ".wkt";
		std::ofstream polygonWKTFile(inputPolygonsWKTStorageLocation + "/" + polygonWKTFileName.str());
		polygonWKTFile << boost_geo::wkt(polygon) << std::endl;
		polygonWKTFile.close();
		// add polygon area to totalAreaOfInputPolygons
		totalAreaOfInputPolygons += std::fabs(boost_geo::area(polygon));
	}

	MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
}