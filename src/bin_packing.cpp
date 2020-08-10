#include <libnfporb.hpp>
#include <bin_packing.hpp>

using namespace libnfporb;

/** namespace polygon_fit */

void polygon_fit::generateAllPairNfpForInputPolygons(string datasetname, int numberOfPolygons, string outputLocation)
{
	// read input polygons wkt files
	vector<polygon_t> polygons(numberOfPolygons);
	for (int i = 0; i < numberOfPolygons; i++)
	{
		std::ostringstream polygonWKTFileName;
		polygonWKTFileName << "polygon_" << std::setw(3) << std::setfill('0') << i << ".wkt";
		string polygonWKTFileNameStr = outputLocation + "/" + datasetname + "/input_polygons_wkt_files/" + polygonWKTFileName.str();
		read_wkt_polygon(polygonWKTFileNameStr, polygons[i]);
		boost_geo::correct(polygons[i]);
	}

	// create directory to store all pair nfp's
	string nfpDirectoryName = outputLocation + "/" + datasetname + "/all_pair_nfp_wkt_files";
	mkdir(nfpDirectoryName.c_str(), 0777);

	int nfp_wkt_file_id = 0;
	for (int i = 0; i < numberOfPolygons; i++)
	{
		for (int j = 0; j < numberOfPolygons; j++)
		{
			for (auto &rotationAngle_i : ALLOWABLE_ROTATIONS)
			{
				for (auto &rotationAngle_j : ALLOWABLE_ROTATIONS)
				{
					polygon_t polygon_i = polygons[i];
					polygon_t polygon_j = polygons[j];
					// std::cout << boost_geo::wkt(polygon_i) << "\n";
					// std::cout << boost_geo::wkt(polygon_j) << "\n\n";
					auto nfp = generateNFP(polygon_i, polygon_j);
					std::reverse(nfp.begin(), nfp.end());
					polygon_t nfpPolygon;
					nfpPolygon.outer() = nfp.back();
					nfp.pop_back();
					nfpPolygon.inners() = nfp;
					std::ofstream nfpWKTFile(nfpDirectoryName + "/nfp_" + std::to_string(nfp_wkt_file_id++) + ".wkt");
					nfpWKTFile << boost_geo::wkt(nfp.front()) << std::endl;
					nfpWKTFile.close();
				}
			}
		}
	}
}

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

	// start the clock
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
		polygonWKTFileName << "polygon_" << std::setw(3) << std::setfill('0') << i << ".wkt";
		std::ofstream polygonWKTFile(inputPolygonsWKTStorageLocation + "/" + polygonWKTFileName.str());
		polygonWKTFile << boost_geo::wkt(polygon) << std::endl;
		polygonWKTFile.close();
		// add polygon area to totalAreaOfInputPolygons
		totalAreaOfInputPolygons += std::fabs(boost_geo::area(polygon));
	}

	polygon_fit::generateAllPairNfpForInputPolygons(datasetName, polygons.size(), outputLocation);

	// MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
}