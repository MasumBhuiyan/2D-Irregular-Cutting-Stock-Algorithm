#include <bin_packing.hpp>
#include <libnfporb.hpp>

using namespace libnfporb;

namespace poly_t_util
{
	polygon_t convertToPolygon_t(Polygon &polygon)
	{
		polygon_t convertedPolygon;
		for (auto &point : polygon.outer())
		{
			long double __x = geo_util::dblround(point.get<0>());
			long double __y = geo_util::dblround(point.get<1>());
			convertedPolygon.outer().push_back(point_t(__x, __y));
		}
		for (auto &inner : polygon.inners())
		{
			vector<point_t> innerNew;
			for (auto &point : inner)
			{
				long double __x = geo_util::dblround(point.get<0>());
				long double __y = geo_util::dblround(point.get<1>());
				innerNew.push_back(point_t(__x, __y));
			}
			convertedPolygon.inners().push_back({innerNew.begin(), innerNew.end()});
		}
		boost_geo::correct(convertedPolygon);
		return convertedPolygon;
	}

	Polygon convertToPolygon(polygon_t &polygon)
	{
		Polygon convertedPolygon;
		for (auto &point : polygon.outer())
		{
			long double __x = geo_util::dblround(point.x_.val());
			long double __y = geo_util::dblround(point.y_.val());
			convertedPolygon.outer().push_back(Point(__x, __y));
		}
		for (auto &inner : polygon.inners())
		{
			vector<Point> innerNew;
			for (auto &point : inner)
			{
				long double __x = geo_util::dblround(point.x_.val());
				long double __y = geo_util::dblround(point.y_.val());
				innerNew.push_back(Point(__x, __y));
			}
			convertedPolygon.inners().push_back({innerNew.begin(), innerNew.end()});
		}
		boost_geo::correct(convertedPolygon);
		return convertedPolygon;
	}
}; // namespace poly_t_util

/** namespace geo_util */
long double geo_util::dblround(long double x, long double eps)
{
	long double intPart = (long long)x;
	long double frcPart = x - intPart;
	if (std::fabs(frcPart) < eps)
	{
		return intPart;
	}
	return x;
}

void geo_util::poly_util::polygonRound(Polygon &polygon)
{
	for(auto &point: polygon.outer())
	{
		double _x = geo_util::dblround(point.get<0>());
		double _y = geo_util::dblround(point.get<1>());
		point = Point(_x, _y);
	}
	for(auto &inner: polygon.inners())
	{
		for(auto &point: inner)
		{
			double _x = geo_util::dblround(point.get<0>());
			double _y = geo_util::dblround(point.get<1>());
			point = Point(_x, _y);
		}
	}
}

Polygon geo_util::poly_util::normalize(Polygon &polygon)
{
	long double min_x = INF, min_y = INF;
	for (auto point : polygon.outer())
	{
		min_x = std::min(min_x, point.get<0>());
		min_y = std::min(min_y, point.get<1>());
	}
	for (auto inner : polygon.inners())
	{
		for (auto point : inner)
		{
			min_x = std::min(min_x, point.get<0>());
			min_y = std::min(min_y, point.get<1>());
		}
	}
	return geo_util::poly_util::translate(polygon, Point(-min_x, -min_y));
}

Polygon geo_util::poly_util::translate(Polygon &polygon, Point translationPoint)
{
	Polygon translatedPolygon;
	boost_geo::transform(polygon, translatedPolygon, trans::translate_transformer<long double, 2, 2>(translationPoint.get<0>(), translationPoint.get<1>()));
	return translatedPolygon;
}

Polygon geo_util::poly_util::rotateCW(Polygon &polygon, long double rotationAngleInDegree, Point referencePoint)
{
	boost_geo::multiply_value(referencePoint, -1);
	Polygon translatedPolygon = geo_util::poly_util::translate(polygon, referencePoint), rotatedPolygon;
	boost_geo::transform(translatedPolygon, rotatedPolygon, trans::rotate_transformer<boost_geo::degree, long double, 2, 2>(rotationAngleInDegree));
	return rotatedPolygon;
}

/** namespace polygon_fit */

Polygon polygon_fit::getNoFitPolygon(Polygon referencePolygon, MultiPolygon cluster)
{
	Polygon nfp = polygon_fit::getNoFitPolygon(referencePolygon, cluster[0]);;
	if( cluster.size() == 2 )
	{
		nfp = polygon_fit::getNoFitPolygon(nfp, cluster[1]);
	}
	return nfp;
}

Polygon polygon_fit::getNoFitPolygon(Polygon referencePolygon, Polygon polygonToPlace)
{
	Polygon nfp;
	return nfp;
}

void polygon_fit::generateAllPairNfpForInputPolygons(vector<Polygon> &polygons, string datasetname, string outputLocation)
{
	int numberOfPolygons = polygons.size();

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
					Polygon polygon_i = polygons[i];
					Polygon polygon_j = polygons[j];

					polygon_i = geo_util::poly_util::rotateCW(polygon_i, rotationAngle_i, polygon_i.outer()[0]);
					Point newOrigin_i = polygon_i.outer().front();
					boost_geo::multiply_value(newOrigin_i, -1);
					polygon_i = geo_util::poly_util::translate(polygon_i, newOrigin_i);
					polygon_i = geo_util::poly_util::translate(polygon_i, Point(10000, 10000));

					polygon_j = geo_util::poly_util::rotateCW(polygon_j, rotationAngle_j, polygon_j.outer()[0]);
					Point newOrigin_j = polygon_j.outer().front();
					boost_geo::multiply_value(newOrigin_j, -1);
					polygon_j = geo_util::poly_util::translate(polygon_j, newOrigin_j);
					polygon_j = geo_util::poly_util::translate(polygon_j, Point(10000, 10000));

					polygon_t polygonT_i = poly_t_util::convertToPolygon_t(polygon_i);
					polygon_t polygonT_j = poly_t_util::convertToPolygon_t(polygon_j);

					nfp_t nfp = generateNFP(polygonT_i, polygonT_j);
					polygon_t nfpPolygon = nfpRingsToNfpPoly(nfp);
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

	polygon_fit::generateAllPairNfpForInputPolygons(polygons, datasetName, outputLocation);

	// MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
}