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
int geo_util::dblcmp(long double d, long double eps)
{
	if (std::fabs(d) < eps)
	{
		return 0;
	}
	return d > eps ? 1 : -1;
}

long double geo_util::dblround(long double x, long double eps)
{
	long double intPart = (long long)x;
	long double frcPart = x - intPart;

	if( std::fabs(x) < eps )
	{
		return 0.0;
	}
	if (std::fabs(frcPart) < eps)
	{
		return intPart;
	}
	return x;
}

long double geo_util::crossProduct(Point p, Point q)
{
	return p.get<0>() * q.get<1>() - p.get<1>() * q.get<0>();
}

int geo_util::orientation(Point a, Point b, Point c)
{
	long double x1 = b.get<0>(), y1 = b.get<1>();
	long double x2 = c.get<0>(), y2 = c.get<1>();
	long double _x = a.get<0>(), _y = a.get<1>();
	long double p = crossProduct(Point(x1 - _x, y1 - _y), Point(x2 - _x, y2 - _y));
	if (p == 0)
	{
		return 0;
	}
	return geo_util::dblcmp(p - EPS) < 0 ? -1 : 1;
}

long double geo_util::linePointDistance(Point a, Point b, Point p)
{
	long double x1 = a.get<0>(), y1 = a.get<1>();
	long double x2 = b.get<0>(), y2 = b.get<1>();
	long double _a = y1 - y2, _b = x2 - x1, _c = x1 * y2 - x2 * y1;
	long double distance = std::fabs(_a * p.get<0>() + _b * p.get<1>() + _c) / sqrt(_a * _a + _b * _b);
	return distance;
}

Point geo_util::segmentSegmentIntersectionPoint(Point a, Point b, Point c, Point d)
{
	int o1 = geo_util::orientation(a, b, c), o2 = geo_util::orientation(a, b, d);
	int o3 = geo_util::orientation(c, d, a), o4 = geo_util::orientation(c, d, b);


	if(  (!o1 && geo_util::pointInRectangle(a, b, c)) )
	{
		return c;
	}
	if( (!o2 && geo_util::pointInRectangle(a, b, d)) )
	{
		return d;
	}
	if((!o3 && geo_util::pointInRectangle(c, d, a)))
	{
		return a;
	}
	if( (!o4 && geo_util::pointInRectangle(c, d, b)))
	{
		return b;
	}
	if ((o1 * o2 == -1 && o3 * o4 == -1))
	{
		long double a1 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(a.get<0>() - c.get<0>(), a.get<1>() - c.get<1>()));
		long double a2 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(b.get<0>() - d.get<0>(), b.get<1>() - d.get<1>()));
		return Point((a.get<0>() * a2 - b.get<0>() * a1) / (a2 - a1), (a.get<1>() * a2 - b.get<1>() * a1) / (a2 - a1));
	}
	return Point(INF, INF);
}
bool geo_util::pointInRectangle(Point a, Point b, Point c)
{
	bool p = (std::min(a.get<0>(), b.get<0>()) <= c.get<0>() && std::max(a.get<0>(), b.get<0>()) >= c.get<0>());
	bool q = (std::min(a.get<1>(), b.get<1>()) <= c.get<1>() && std::max(a.get<1>(), b.get<1>()) >= c.get<1>());
	return (p && q);
}

/** namespace geo_uti::poly_util */

void geo_util::poly_util::readWKTPolygon(Polygon &polygon, string filename)
{
	std::ifstream polygonWKTFile(filename);
	assert(polygonWKTFile.is_open());

	string wktStr;
	polygonWKTFile.seekg(0, std::ios::end);
	wktStr.reserve(polygonWKTFile.tellg());
	polygonWKTFile.seekg(0, std::ios::beg);
	wktStr.assign((std::istreambuf_iterator<char>(polygonWKTFile)), std::istreambuf_iterator<char>());
	wktStr.pop_back();
	boost_geo::read_wkt(wktStr, polygon);
	boost_geo::correct(polygon);

	polygonWKTFile.close();
}

void geo_util::poly_util::polygonRound(Polygon &polygon)
{
	for (auto &point : polygon.outer())
	{
		double _x = geo_util::dblround(point.get<0>());
		double _y = geo_util::dblround(point.get<1>());
		point = Point(_x, _y);
	}
	for (auto &inner : polygon.inners())
	{
		for (auto &point : inner)
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

MultiPolygon geo_util::poly_util::translate(MultiPolygon &polygons, Point translationPoint)
{
	MultiPolygon translatedPolygons = polygons;
	for (auto &polygon : translatedPolygons)
	{
		polygon = geo_util::poly_util::translate(polygon, translationPoint);
	}
	return translatedPolygons;
}

Polygon geo_util::poly_util::rotateCW(Polygon &polygon, long double rotationAngleInDegree, Point referencePoint)
{
	boost_geo::multiply_value(referencePoint, -1);
	Polygon translatedPolygon = geo_util::poly_util::translate(polygon, referencePoint), rotatedPolygon;
	boost_geo::transform(translatedPolygon, rotatedPolygon, trans::rotate_transformer<boost_geo::degree, long double, 2, 2>(rotationAngleInDegree));
	return rotatedPolygon;
}

MultiPolygon geo_util::poly_util::rotateCW(MultiPolygon multiPolygon, long double rotationAngleInDegree, Point referencePoint)
{
	for(auto &polygon: multiPolygon)
	{
		polygon = rotateCW(polygon, rotationAngleInDegree, referencePoint);
	}
	return multiPolygon;
}
long double geo_util::poly_util::polygonPolygonIntersectionArea(Polygon &polygon1, Polygon &polygon2)
{
	std::deque<Polygon> polygons;
	boost_geo::intersection(polygon1, polygon2, polygons);
	long double intersectedArea = 0.0;
	for (auto polygon : polygons)
	{
		intersectedArea += boost_geo::area(polygon);
	}
	return std::fabs(intersectedArea);
}

bool geo_util::poly_util::isItPossibleToPlacePolygon(MultiPolygon &packing, MultiPolygon clusterNextToBePlaced, Point point)
{
	Point referecePoint = clusterNextToBePlaced[0].outer()[0];
	boost_geo::multiply_value(referecePoint, -1);
	for (auto &polygon : clusterNextToBePlaced)
	{
		polygon = geo_util::poly_util::translate(polygon, referecePoint);
		polygon = geo_util::poly_util::translate(polygon, point);
	}
	for (auto poly : packing)
	{
		for (auto polygonNextToBePlaced : clusterNextToBePlaced)
		{
			bool doesOverlap = boost_geo::overlaps(poly, polygonNextToBePlaced);
			if( doesOverlap == true ) 
			{
				return false;
			}
			if ( geo_util::dblcmp(geo_util::poly_util::polygonPolygonIntersectionArea(polygonNextToBePlaced, poly), EPS) > 0)
			{
				return false;
			}
		}
	}
	return true;
}

long double geo_util::poly_util::getWidth(MultiPolygon &multiPolygon)
{
	long double min_x = INF, max_x = -INF;
	for (auto polygon : multiPolygon)
	{
		for (auto point : polygon.outer())
		{
			min_x = std::min(point.get<0>(), min_x);
			max_x = std::max(point.get<0>(), max_x);
		}
	}
	return max_x - min_x;
}

long double geo_util::poly_util::getLength(MultiPolygon &multiPolygon)
{
	long double min_y = INF, max_y = -INF;
	for (auto polygon : multiPolygon)
	{
		for (auto point : polygon.outer())
		{
			min_y = std::min(point.get<1>(), min_y);
			max_y = std::max(point.get<1>(), max_y);
		}
	}
	return max_y - min_y;
}

void geo_util::visualize(MultiPolygon multiPolygon, string outputLocation, string datasetName)
{
	Box box;
	boost::geometry::envelope(multiPolygon, box);
	std::cout << "make_envelope..............: " << boost::geometry::dsv(box) << std::endl;
	std::ostringstream name;
	name << "frame_" << std::setw(4) << std::setfill('0') << frameno++ << "_" << datasetName << ".svg";
	std::ofstream svg(outputLocation + "/" + name.str());
	boost_geo::svg_mapper<Point> mapper(svg, 600, 700);
	mapper.add(multiPolygon);
	mapper.map(multiPolygon, "fill-opacity:0.5;fill:rgb(169,169,169);stroke:rgb(169,169,169);stroke-width:1");
	mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(0,0,0);stroke-width:1;stroke-linecap:round");
}

void geo_util::visualizeCluster(Polygon polygon, MultiPolygon cluster, Polygon nfp, std::string location, int testId)
{
    Box box;
    MultiPolygon multipolygon;
	multipolygon.push_back(cluster[0]);
	if( cluster.size() == 2 ) multipolygon.push_back(cluster[1]);
    std::ostringstream name;
    name << "test" << std::setw(2) << std::setfill('0') << testId << ".svg";
    std::ofstream svg(location + name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 300, 300, "width=\"200mm\" height=\"200mm\" viewBox=\"-250 -250 800 800\"");
    mapper.add(nfp);
    mapper.map(nfp, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:2");
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(212, 0, 0);stroke:rgb(212,0,0);stroke-width:2");
    mapper.add(polygon);
    mapper.map(polygon, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
}
/** namespace polygon_fit */

Polygon polygon_fit::getInnerFitRectangle(MultiPolygon cluster, long double length, long double width)
{
	Polygon innerFitRectangle;
	Point reference = cluster[0].outer()[0];

	long double max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
	for (Polygon &polygon : cluster)
	{
		for (Point &point : polygon.outer())
		{
			point = Point(point.get<0>() - reference.get<0>(), point.get<1>() - reference.get<1>());
			max_x = std::max(max_x, point.get<0>());
			min_x = std::min(min_x, point.get<0>());
			max_y = std::max(max_y, point.get<1>());
			min_y = std::min(min_y, point.get<1>());
		}
	}
	innerFitRectangle.outer().push_back(Point(std::abs(min_x), std::abs(min_y)));
	innerFitRectangle.outer().push_back(Point(std::abs(min_x), length - max_y));
	innerFitRectangle.outer().push_back(Point(width - max_x, length - max_y));
	innerFitRectangle.outer().push_back(Point(width - max_x, std::abs(min_y)));
	innerFitRectangle.outer().push_back(Point(std::abs(min_x), std::abs(min_y)));
	return innerFitRectangle;
}

vector<Point> polygon_fit::getAllEdgeIntersectionPoints(MultiPolygon &allNfpIfr)
{
	vector<tuple<Point, Point>> edges;
	vector<Point> allEdgeIntersectionPoints;

	for (auto &polygon : allNfpIfr)
	{
		for (int i = 0; i + 1 < polygon.outer().size(); i += 1)
		{
			edges.push_back({polygon.outer()[i], polygon.outer()[i + 1]});
		}
	}
	for (int i = 0; i < edges.size(); i += 1)
	{
		for (int j = i + 1; j < edges.size(); j += 1)
		{
			Point point1, point2, point3, point4;
			std::tie(point1, point2) = edges[i];
			std::tie(point3, point4) = edges[j];
			Point inetersectionPoint = geo_util::segmentSegmentIntersectionPoint(point1, point2, point3, point4);
			if (inetersectionPoint.get<0>() == INF and inetersectionPoint.get<1>() == INF)
			{
				continue;
			}
			allEdgeIntersectionPoints.push_back(inetersectionPoint);
		}
	}
	return allEdgeIntersectionPoints;
}

Polygon polygon_fit::getNoFitPolygon(Polygon referencePolygon, MultiPolygon cluster)
{
	referencePolygon = geo_util::poly_util::translate(referencePolygon, Point(10000,10000));
	cluster[0] = geo_util::poly_util::translate(cluster[0], Point(10000,10000));
	// std::cout << "blf no fit: " << boost_geo::wkt(referencePolygon) << "\n"; 
	// std::cout << "cluster1: " << boost_geo::wkt(cluster[0]) << "\n"; 
	Polygon nfp = polygon_fit::getNoFitPolygon(referencePolygon, cluster[0]);
	if (cluster.size() == 2)
	{
		cluster[1] = geo_util::poly_util::translate(cluster[1], Point(10000,10000));
		// std::cout << "cluster2: " << boost_geo::wkt(cluster[1]) << "\n"; 
		nfp = polygon_fit::getNoFitPolygon(nfp, cluster[1]);
	}
	nfp = geo_util::poly_util::translate(nfp, Point(-10000,-10000));
	//std::cout << boost_geo::wkt(nfp) << '\n';
	//std::cout << "end....\n";
	return nfp;
}

Polygon polygon_fit::getNoFitPolygon(Polygon referencePolygon, Polygon polygonToPlace)
{
	Polygon nfpPolygon;
	if( boost_geo::is_valid(referencePolygon) == false ) return nfpPolygon;
	if( boost_geo::is_valid(polygonToPlace) == false ) return nfpPolygon;
	geo_util::poly_util::polygonRound(referencePolygon);
	geo_util::poly_util::polygonRound(polygonToPlace);
	referencePolygon = geo_util::poly_util::translate(referencePolygon, Point(10000, 10000));
	polygonToPlace = geo_util::poly_util::translate(polygonToPlace, Point(10000, 10000));
	polygon_t polygonT_i = poly_t_util::convertToPolygon_t(referencePolygon);
	polygon_t polygonT_j = poly_t_util::convertToPolygon_t(polygonToPlace);

	nfp_t nfp = generateNFP(polygonT_i, polygonT_j);
	polygon_t nfpPolygonT = nfpRingsToNfpPoly(nfp);
	nfpPolygon = poly_t_util::convertToPolygon(nfpPolygonT);
	nfpPolygon = geo_util::poly_util::translate(nfpPolygon, Point(-10000, -10000));
	geo_util::poly_util::polygonRound(nfpPolygon);
	boost_geo::correct(nfpPolygon);
	
	return nfpPolygon;
}

MultiPolygon polygon_fit::getAllNfpIfr(MultiPolygon &packing, MultiPolygon cluster, long double length, long double width)
{
	MultiPolygon allNfpIfr;
	Polygon ifr = polygon_fit::getInnerFitRectangle(cluster, length, width);
	allNfpIfr.push_back(ifr);
	for (auto &polygon : packing)
	{
		Polygon nfp = polygon_fit::getNoFitPolygon(polygon, cluster);
		allNfpIfr.push_back(nfp);
	}
	return allNfpIfr;
}

/** namespace cluster_util */
vector<tuple<int, int, int, int>> cluster_util::getPerfectClustering(vector<vector<vector<vector<long double>>>> &clusterValues)
{
	int numberOfPolygons = clusterValues.size();
	int numberOfPairs = std::min(10, numberOfPolygons / 2);
	std::cout << "number of pairs: " << numberOfPairs << std::endl;
	vector<tuple<int, int, int, int>> clusterPairs;
	vector<long double> dp(1 << 22, -1.0);
	long double bestClusterizationValue = cluster_util::findBestPairWiseClusters(clusterValues, dp, numberOfPairs, 0);
	cluster_util::printBestPairWiseClusters(clusterValues, dp, numberOfPairs, 0, clusterPairs);
	return clusterPairs;
}

void cluster_util::printBestPairWiseClusters(vector<vector<vector<vector<long double>>>> &clusterValues, vector<long double> &dp, int numberOfPairs, int mask, vector<tuple<int, int, int, int>> &clusterPairs)
{
	int taken = __builtin_popcount(mask);
	if (taken == (numberOfPairs + numberOfPairs))
	{
		return;
	}
	long double &maxProfit = dp[mask];
	int POLYGON_SIZE = std::min(20, (int)clusterValues.size());
	for (int i = 0; i < POLYGON_SIZE; i += 1)
	{
		int bit1 = mask & (1 << i);
		if (bit1 > 0)
		{
			continue;
		}
		for (int j = 0; j < POLYGON_SIZE; j += 1)
		{
			int bit2 = mask & (1 << j);
			if (bit2 > 0 or i == j)
			{
				continue;
			}
			for (int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
			{
				for (int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
				{
					int bitOn12 = (1 << i) + (1 << j);
					long double __maxProfit = cluster_util::findBestPairWiseClusters(clusterValues, dp, numberOfPairs, mask ^ bitOn12) + clusterValues[i][j][k][l];
					if (maxProfit == __maxProfit)
					{
						clusterPairs.push_back({i, j, k, l});
						cluster_util::printBestPairWiseClusters(clusterValues, dp, numberOfPairs, mask ^ bitOn12, clusterPairs);
						return;
					}
				}
			}
		}
	}
}

long double cluster_util::findBestPairWiseClusters(vector<vector<vector<vector<long double>>>> &clusterValues, vector<long double> &dp, int numberOfPairs, int mask)
{
	int taken = __builtin_popcount(mask);
	if (taken == (numberOfPairs + numberOfPairs))
	{
		return 0.0;
	}
	long double &maxProfit = dp[mask];
	if (maxProfit != -1.0)
	{
		return maxProfit;
	}
	maxProfit = -INF;

	int POLYGON_SIZE = std::min(20, (int)clusterValues.size());
	for (int i = 0; i < POLYGON_SIZE; i += 1)
	{
		int bit1 = mask & (1 << i);
		if (bit1 > 0)
		{
			continue;
		}
		for (int j = 0; j < POLYGON_SIZE; j += 1)
		{
			int bit2 = mask & (1 << j);
			if (bit2 > 0 or i == j)
			{
				continue;
			}
			for (int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
			{
				for (int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
				{
					int bitOn12 = (1 << i) + (1 << j);
					long double __maxProfit = cluster_util::findBestPairWiseClusters(clusterValues, dp, numberOfPairs, mask ^ bitOn12) + clusterValues[i][j][k][l];
					maxProfit = std::max(maxProfit, __maxProfit);
				}
			}
		}
	}
	return maxProfit;
}

vector<Point> cluster_util::getCandidatePlacementPositions(MultiPolygon &packing, MultiPolygon &cluster, long double length, long double width)
{
	vector<Point> candidatePlacementPositions;
	MultiPolygon allNfpIfr = polygon_fit::getAllNfpIfr(packing, cluster, length, width);
	vector<Point> allEdgeIntersectionPoints = polygon_fit::getAllEdgeIntersectionPoints(allNfpIfr);


	double min_x = INF, min_y = INF, max_x = -INF, max_y = -INF;

	Point referencePoint = cluster[0].outer()[0];
	for(auto polygon: cluster)
	{
		for(auto point: polygon.outer())
		{
			double _x = point.get<0>() - referencePoint.get<0>();
			double _y = point.get<1>() - referencePoint.get<1>();
			min_x = std::min(min_x, _x);
			max_x = std::max(max_x, _x);
			min_y = std::min(min_y, _y);
			max_y = std::max(max_y, _y);
		}
	} 

	for (auto polygon : allNfpIfr)
	{
		for (auto point : polygon.outer())
		{
			double _x = point.get<0>();
			double _y = point.get<1>();
			if (_x + min_x < 0 or _x + max_x > width or _y + min_y < 0 or _y + max_y > length)
			{
				continue;
			}
			candidatePlacementPositions.push_back(point);
		}
		for (auto inner : polygon.inners())
		{
			for (auto point : inner)
			{
				double _x = point.get<0>();
				double _y = point.get<1>();
				if (_x + min_x < 0 or _x + max_x > width or _y + min_y < 0 or _y + max_y > length)
				{
					continue;
				}
				candidatePlacementPositions.push_back(point);
			}
		}
	}
	for (auto point : allEdgeIntersectionPoints)
	{
		double _x = point.get<0>();
		double _y = point.get<1>();
		if (_x + min_x < 0 or _x + max_x > width or _y + min_y < 0 or _y + max_y > length)
		{
			continue;
		}
		candidatePlacementPositions.push_back(point);
	}
	// assert(candidatePlacementPositions.size() > 0);
	return candidatePlacementPositions;
}

Point cluster_util::findBLFPoint(MultiPolygon &packing, MultiPolygon &cluster, long double length, long double width)
{
	Point blfPoint(INF, INF);
	vector<Point> candidatePlacementPositions = cluster_util::getCandidatePlacementPositions(packing, cluster, length, width);
	for (auto point : candidatePlacementPositions)
	{
		if (geo_util::poly_util::isItPossibleToPlacePolygon(packing, cluster, point) == true)
		{
			if ( geo_util::dblcmp(point.get<1>() - blfPoint.get<1>(), EPS) < 0 )
			{
				blfPoint = point;
			}
			else if (geo_util::dblcmp(point.get<1>() - blfPoint.get<1>(), EPS) == 0 and geo_util::dblcmp(point.get<0>() - blfPoint.get<0>(), EPS) < 0)
			{
				blfPoint = point;
			}
		}
	}
	// assert(blfPoint.get<0>() != INF);
	return blfPoint;
}

MultiPolygon cluster_util::bottomLeftFill(vector<MultiPolygon> &clusters, long double length, long double width)
{
	MultiPolygon packing;
	for (auto cluster : clusters)
	{

		vector<tuple<int, long double, long double>> values;
		for(int i = 0; i < ALLOWABLE_ROTATIONS.size(); i += 1)
		{
			MultiPolygon temporaryCluster = geo_util::poly_util::rotateCW(cluster, ALLOWABLE_ROTATIONS[i], cluster[0].outer()[0]);
			Point temporaryBlfPoint = cluster_util::findBLFPoint(packing, temporaryCluster, length, width);
			
			Point temporaryReferencePoint = temporaryCluster.front().outer().front();
			boost_geo::multiply_value(temporaryReferencePoint, -1);

			temporaryCluster = geo_util::poly_util::translate(temporaryCluster, temporaryReferencePoint);
			temporaryCluster = geo_util::poly_util::translate(temporaryCluster, temporaryBlfPoint);

			if( temporaryBlfPoint.get<0>() == INF ) continue;
			if( temporaryBlfPoint.get<1>() == INF ) continue;
			
			for (auto polygon : temporaryCluster)
			{
				packing.push_back(polygon);
			}
			long double packingLength = geo_util::poly_util::getLength(packing);
			long double packingWidth = geo_util::poly_util::getWidth(packing);
			for (auto polygon : temporaryCluster)
			{
				packing.pop_back();
			}

			values.push_back({i, packingLength, packingWidth});
		}
		assert(values.size() > 0);
		sort(values.begin(), values.end(), [](std::tuple<int,long double,long double> value1, std::tuple<int, long double, long double> value2){
			int rotationId1, rotationId2;
			double packingLength1, packingLength2, packingWidth1, packingWidth2;

			std::tie(rotationId1, packingLength1, packingWidth1) = value1;
			std::tie(rotationId2, packingLength2, packingWidth2) = value2;
			if( geo_util::dblcmp(packingLength1 - packingLength2) < 0 ) return true;
			if( geo_util::dblcmp(packingLength1 - packingLength2) > 0 ) return false;
			if( geo_util::dblcmp(packingLength1 - packingLength2) == 0 && geo_util::dblcmp(packingWidth1 - packingWidth2) < 0 ) return true;
			if( geo_util::dblcmp(packingLength1 - packingLength2) == 0 && geo_util::dblcmp(packingWidth1 - packingWidth2) > 0 ) return false;
			return true;
		});

		int rotationId;
		double bestPackingLength;
		double bestPackingWidth;
		std::tie(rotationId, bestPackingLength, bestPackingWidth) = values[0];
		cluster = geo_util::poly_util::rotateCW(cluster, ALLOWABLE_ROTATIONS[rotationId], cluster[0].outer()[0]);

		Point blfPoint = cluster_util::findBLFPoint(packing, cluster, length, width);
		Point referencePoint = cluster.front().outer().front();
		boost_geo::multiply_value(referencePoint, -1);

		cluster = geo_util::poly_util::translate(cluster, referencePoint);
		cluster = geo_util::poly_util::translate(cluster, blfPoint);

		for (auto polygon : cluster)
		{
			packing.push_back(polygon);
		}
	}
	return packing;
}

void cluster_util::sortByClusterValue(vector<MultiPolygon> &clusters)
{
	sort(clusters.begin(), clusters.end(), [](MultiPolygon cluster1, MultiPolygon cluster2) {
		long double area1 = 0.0, area2 = 0.0;
		for (auto polygon : cluster1)
		{
			area1 += std::fabs(boost_geo::area(polygon));
		}
		for (auto polygon : cluster2)
		{
			area2 += std::fabs(boost_geo::area(polygon));
		}
		return geo_util::dblcmp(area1 - area2, EPS) > 0;
	});
}

Polygon cluster_util::convexHull(MultiPolygon multiPolygon)
{
	Polygon convexhull;
	boost_geo::convex_hull(multiPolygon, convexhull);
	return convexhull;
}

MultiPolygon cluster_util::findConvexHullVacancy(Polygon &concavePolygon)
{
	MultiPolygon vacancies;
	if (boost_geo::is_convex(concavePolygon.outer()) == true)
		return vacancies;
	Polygon convexhull = cluster_util::convexHull({concavePolygon});
	boost_geo::difference(convexhull, concavePolygon, vacancies);
	return vacancies;
}

vector<tuple<Point, Point>> cluster_util::findOppositeSideOfVacancies(Polygon &concavePolygonConvexHull, MultiPolygon &convexHullVacancies)
{
	vector<tuple<Point, Point>> oppositeSideOfVacancies;
	if (convexHullVacancies.size() == 0)
	{
		return oppositeSideOfVacancies;
	}
	int i = 0;
	for (auto &vacancy : convexHullVacancies)
	{
		vector<Point> line;
		boost_geo::intersection(concavePolygonConvexHull, vacancy, line);
		assert(line.size() == 2);
		oppositeSideOfVacancies.push_back({line[0], line[1]});
	}
	return oppositeSideOfVacancies;
}

Point cluster_util::findDominantPoint(Polygon &concavePolygon)
{
	Point dominant;
	Polygon convexhull = cluster_util::convexHull({concavePolygon});
	MultiPolygon vacancies = cluster_util::findConvexHullVacancy(concavePolygon);
	vector<tuple<Point, Point>> oppositeSideOfVacancies = cluster_util::findOppositeSideOfVacancies(convexhull, vacancies);
	int n = oppositeSideOfVacancies.size();
	double maxDistance = 0.0;

	for (int i = 0; i < n; i += 1)
	{
		for (Point point : vacancies[i].outer())
		{
			Point p, q;
			std::tie(p, q) = oppositeSideOfVacancies[i];
			long double distance = geo_util::linePointDistance(p, q, point);
			if (distance > maxDistance)
			{
				maxDistance = distance;
				dominant = point;
			}
		}
	}
	return dominant;
}

long double cluster_util::getClusteringCriteria1(Polygon &polygon1, Polygon &polygon2)
{
	long double value = 0.0, value1 = 0.0, value2 = 0.0;
	long double areaIntersection1 = 0.0, totalVacancy1 = 0.0;
	long double areaIntersection2 = 0.0, totalVacancy2 = 0.0;

	vector<Polygon> V1 = cluster_util::findConvexHullVacancy(polygon1);
	vector<Polygon> V2 = cluster_util::findConvexHullVacancy(polygon2);

	for (auto &vacancy : V2)
	{
		long double area = geo_util::poly_util::polygonPolygonIntersectionArea(polygon1, vacancy);
		if (area > 0)
		{
			areaIntersection1 += area;
			totalVacancy1 += std::fabs(boost_geo::area(vacancy));
		}
	}
	for (auto &vacancy : V1)
	{
		long double area = geo_util::poly_util::polygonPolygonIntersectionArea(polygon2, vacancy);
		if (area > 0)
		{
			areaIntersection2 += area;
			totalVacancy2 += std::fabs(boost_geo::area(vacancy));
		}
	}
	if (totalVacancy1 > 0)
	{
		value1 = areaIntersection1 / totalVacancy1;
	}
	if (totalVacancy2 > 0)
	{
		value2 = areaIntersection2 / totalVacancy2;
	}
	value = std::max(value1, value2);
	return value;
}

long double cluster_util::getClusteringCriteria2(Polygon &polygon1, Polygon &polygon2)
{
	MultiPolygon multiPolygon;
	multiPolygon.push_back(polygon1);
	multiPolygon.push_back(polygon2);
	Polygon convexhull = cluster_util::convexHull(multiPolygon);

	long double area1 = std::fabs(boost_geo::area(polygon1));
	long double area2 = std::fabs(boost_geo::area(polygon2));
	long double area3 = std::fabs(boost_geo::area(convexhull));
	assert(area3 > 0);
	long double value = (area1 + area2) / area3;
	return value;
}

long double cluster_util::getClusterValue(Polygon &polygon1, Polygon &polygon2)
{
	long double value = cluster_util::getClusteringCriteria1(polygon1, polygon2) + cluster_util::getClusteringCriteria2(polygon1, polygon2);
	return value;
}

vector<vector<vector<vector<long double>>>> cluster_util::getClusterValues(vector<Polygon> &inputPolygons)
{
	int numberOfPolygons = inputPolygons.size();
	vector<vector<vector<vector<long double>>>> clusterValues;
	clusterValues.resize(numberOfPolygons, vector<vector<vector<long double>>>(numberOfPolygons, vector<vector<long double>>(ALLOWABLE_ROTATIONS.size(), vector<long double>(ALLOWABLE_ROTATIONS.size(), 0.0))));
	for (int i = 0; i < numberOfPolygons; i++)
	{
		for (int j = 0; j < numberOfPolygons; j++)
		{
			if (i == j)
			{
				continue;
			}
			for (int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
			{
				for (int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
				{
					Polygon polygon_i = inputPolygons[i];
					Polygon polygon_j = inputPolygons[j];

					long double rotationAngle_i = ALLOWABLE_ROTATIONS[k];
					long double rotationAngle_j = ALLOWABLE_ROTATIONS[l];

					polygon_i = geo_util::poly_util::rotateCW(polygon_i, rotationAngle_i, polygon_i.outer()[0]);
					Point newOrigin_i = polygon_i.outer().front();
					boost_geo::multiply_value(newOrigin_i, -1);
					polygon_i = geo_util::poly_util::translate(polygon_i, newOrigin_i);

					polygon_j = geo_util::poly_util::rotateCW(polygon_j, rotationAngle_j, polygon_j.outer()[0]);
					Point newOrigin_j = polygon_j.outer().front();
					boost_geo::multiply_value(newOrigin_j, -1);
					polygon_j = geo_util::poly_util::translate(polygon_j, newOrigin_j);

					int nfp_file_no =(l + k * ALLOWABLE_ROTATIONS.size() + j * ALLOWABLE_ROTATIONS.size() * ALLOWABLE_ROTATIONS.size()  
										+ i * ALLOWABLE_ROTATIONS.size() * ALLOWABLE_ROTATIONS.size() * numberOfPolygons);
					//polygon_t nfpt;
					//std::string filename = "../io/nfpfiles/trousers/" + std::to_string(nfp_file_no) + ".wkt";
					// std::cout << nfp_file_no << "\n";
					//read_wkt_polygon(filename, nfpt);

					//Polygon nfp = poly_t_util::convertToPolygon(nfpt);

					polygon_i = geo_util::poly_util::translate(polygon_i, Point(10000,10000));
					polygon_j = geo_util::poly_util::translate(polygon_j, Point(10000,10000));
					std::cout << boost_geo::wkt(polygon_i) << "\n";
					std::cout << boost_geo::wkt(polygon_j) << "\n";
					Polygon nfp = polygon_fit::getNoFitPolygon(polygon_i, polygon_j);
					polygon_i = geo_util::poly_util::translate(polygon_i, Point(-10000,-10000));
					polygon_j = geo_util::poly_util::translate(polygon_j, Point(-10000,-10000));

					if (boost_geo::is_convex(nfp.outer()) == false)
					{
						Point dominantPoint = cluster_util::findDominantPoint(nfp);
						polygon_j = geo_util::poly_util::translate(polygon_j, dominantPoint);
						clusterValues[i][j][k][l] = cluster_util::getClusterValue(polygon_i, polygon_j);
					}
				}
			}
		}
	}
	return clusterValues;
}

MultiPolygon cluster_util::generateInitialSolution(vector<Polygon> &inputPolygons, long double width)
{
	// std::cout << "Cluster Values calculating..." << "...\n";
	// vector<vector<vector<vector<long double>>>> clusterValues = cluster_util::getClusterValues(inputPolygons);

	// std::cout << "Calling getPerfectClustering..." << "...\n";
	// vector<tuple<int, int, int, int>> clusterPairs = cluster_util::getPerfectClustering(clusterValues);

	vector<bool> taken((int)inputPolygons.size(), false);
	vector<MultiPolygon> clusters;
	int n = inputPolygons.size();

	// int testId = 0;
	// for (auto &clusterId : clusterPairs)
	// {
	// 	int i, j, k, l;
	// 	std::tie(i, j, k, l) = clusterId;
	// 	MultiPolygon cluster;

	// 	Polygon polygon_i = inputPolygons[i];
	// 	Polygon polygon_j = inputPolygons[j];

	// 	long double rotationAngle_i = ALLOWABLE_ROTATIONS[k];
	// 	long double rotationAngle_j = ALLOWABLE_ROTATIONS[l];

	// 	polygon_i = geo_util::poly_util::rotateCW(polygon_i, rotationAngle_i, polygon_i.outer()[0]);
	// 	Point newOrigin_i = polygon_i.outer().front();
	// 	boost_geo::multiply_value(newOrigin_i, -1);
	// 	polygon_i = geo_util::poly_util::translate(polygon_i, newOrigin_i);

	// 	polygon_j = geo_util::poly_util::rotateCW(polygon_j, rotationAngle_j, polygon_j.outer()[0]);
	// 	Point newOrigin_j = polygon_j.outer().front();
	// 	boost_geo::multiply_value(newOrigin_j, -1);
	// 	polygon_j = geo_util::poly_util::translate(polygon_j, newOrigin_j);

	// 	Polygon nfp = polygon_fit::getNoFitPolygon(polygon_i, polygon_j);
	// 	continue;
	// 	if (boost_geo::is_convex(nfp.outer()) == false)
	// 	{
	// 		taken[i] = taken[j] = true;
	// 		Point dominantPoint = cluster_util::findDominantPoint(nfp);

	// 		polygon_j = geo_util::poly_util::translate(polygon_j, dominantPoint);
	// 		cluster.push_back(polygon_i);
	// 		cluster.push_back(polygon_j);
	// 		clusters.push_back(cluster);

	// 		// geo_util::visualizeCluster(polygon_i, {polygon_j}, nfp, "../io/results/", testId++);
	// 	}
	// }

	for (int i = 0; i < inputPolygons.size(); i++)
	{
		if (taken[i] == false)
		{
			clusters.push_back({inputPolygons[i]});
		}
	}

	for(auto cluster: clusters)
	{
		// geo_util::visualize(cluster, "../io/results", "sample");
		//std::cout << boost_geo::wkt(cluster) << "\n";
	}
//	std::cout << "Sorting..." << "...\n";
	cluster_util::sortByClusterValue(clusters);
	long double length = INITIAL_STOCK_LENGTH;
//	std::cout << "Calling BottomLeft Fill:\n";
	MultiPolygon initialSolution = cluster_util::bottomLeftFill(clusters, length, width);
//	std::cout << "generateInitialSolution done..." << "...\n";
	return initialSolution;
}

/** namespace bin_packing */
/** 
 * read input from dataset file
 * file location ../io/datasets/**.txt
 */
std::tuple<vector<Polygon>, long double> bin_packing::readDataset(std::string datasetName)
{
	std::cout << "Reading dataset " << datasetName << "...\n";
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
	file.close();
	std::cout << "Reading completed" << "...\n";
	return {polygons, width};
}

/**
 * checks feasibilty of a packing
 * compares with FEASIBILTY limit
 */
bool bin_packing::isFeasible(MultiPolygon &packing, long double totalAreaOfInputPolygons)
{
	long double overlappingArea = 0.0;
	long double feasibilityRatio = 0.0;
	int n = packing.size();
	for (int i = 0; i < n; i += 1)
	{
		for (int j = i + 1; j < n; j += 1)
		{
			overlappingArea += geo_util::poly_util::polygonPolygonIntersectionArea(packing[i], packing[j]);
		}
	}
	feasibilityRatio = overlappingArea / totalAreaOfInputPolygons;
	return geo_util::dblcmp(feasibilityRatio - FEASIBILTY, EPS) <= 0 ? true : false;
}
/**
 * naive implementation
 * may be changed in future
 */
long double bin_packing::getPenetrationDepth(Polygon polygonA, Polygon polygonB)
{
	MultiPolygon intersections;
	boost_geo::intersection(polygonA, polygonB, intersections);
	return std::fabs(boost_geo::area(intersections));
}
/**
 * returns total penitration depth of a pcking
 */
long double bin_packing::getTotalPenetrationDepth(MultiPolygon &packing)
{
	long double totalPenetrationDepth = 0.0;
	int n = packing.size();
	for (int i = 0; i < n; i += 1)
	{
		for (int j = i + 1; j < n; j += 1)
		{
			totalPenetrationDepth += bin_packing::getPenetrationDepth(packing[i], packing[j]);
		}
	}
	return totalPenetrationDepth;
}

/**
 * 
 */
long double bin_packing::getOverlapPenalty(MultiPolygon &packing, std::vector<std::vector<long double>> &penalty, int id, long double rotationAngle, Point translationPoint)
{
	int n = packing.size();
	Polygon polygon = packing[id];

	// rotating
	polygon = geo_util::poly_util::rotateCW(polygon, rotationAngle, polygon.outer()[0]);

	// translating 
	Point referencePoint = polygon.outer()[0];
	boost_geo::multiply(referencePoint, -1);
	polygon = geo_util::translate(polygon, referencePoint);
	polygon = geo_util::translate(polygon, translationPoint);

	long double overlapPenalty = 0;
	for (int j = 0; j < n; j++)
	{
		if( j != id ) 
		{
			overlapPenalty += (penalty[id][j] * bin_packing::getPenetrationDepth(polygon, packing[j]));
		}
	}
	return overlapPenalty;
}

void bin_packing::increasePenalty(MultiPolygon &packing, std::vector<std::vector<long double>> &penalty)
{
	int n = penalty.size();
	long double maximumPenetrationDepth = -INF;
	std::vector<std::vector<long double>> penetrationDepths(n, std::vector<long double>(n, 0));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if( i == j ) 
			{ 
				continue;
			}
			penetrationDepths[i][j] = bin_packing::getPenetrationDepth(packing[i], packing[j]);
			if (i < j)
			{
				maximumPenetrationDepth = std::max(maximumPenetrationDepth, penetrationDepths[i][j]);
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			penalty[i][j] += (penetrationDepths[i][j] / maximumPenetrationDepth);
		}
	}
}

Point bin_packing::cuckooSearch(MultiPolygon &packing, std::vector<std::vector<long double>> &penalty, int id, long double rotationAngle, long double width, long double length)
{
	Polygon polygon = packing[id];
	polygon = geo_util::poly_util::rotateCW(polygon, rotationAngle, polygon.outer()[0]);
	Polygon innerFitRectangle = polygon_fit::getInnerFitRectangle({polygon}, length, width);

	long double max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
	for (Point &point : polygon.outer())
	{
		max_x = std::max(max_x, point.get<0>());
		min_x = std::min(min_x, point.get<0>());
		max_y = std::max(max_y, point.get<1>());
		min_y = std::min(min_y, point.get<1>());
	}

	std::vector<Point> hostNests(NUMBER_OF_HOST_NESTS);
	for (int i = 0; i < NUMBER_OF_HOST_NESTS; i++)
	{
		srand(time(NULL));
		hostNests[i] = {std::rand() % (int)(max_x - min_x + 1) + min_x, std::rand() % (int)(max_y - min_y + 1) + min_y};
	}
	// Begin cuckoo search
	Point bestPosition = packing[id].outer()[0];
	for (int t = 0; t < MAXIMUM_GENERATION; t++)
	{
		srand(time(NULL));
		int i = std::rand() % NUMBER_OF_HOST_NESTS;
		long double overlapPenalty_i = bin_packing::getOverlapPenalty(packing, penalty, id, rotationAngle, hostNests[i]);

		srand(time(NULL));
		int j = std::rand() % NUMBER_OF_HOST_NESTS;
		long double overlapPenalty_j = bin_packing::getOverlapPenalty(packing, penalty, id, rotationAngle, hostNests[j]);

		if (overlapPenalty_i < overlapPenalty_j)
		{
			hostNests[j] = hostNests[i];
			bestPosition = hostNests[j];
		}
		// Abandon a fraction (Pa) of worse nests and build new ones
		// at new locations
		// Keep the best solutions (or nests with quality solutions)
		// Rank the solutions and find the current best
	}
	return bestPosition;
}

MultiPolygon bin_packing::minimizeOverlap(MultiPolygon packing, std::vector<long double> allowableRoatations, long double width, long double length)
{
	int n = packing.size();
	std::vector<std::vector<long double>> penalty(n, std::vector<long double>(n, 1.0));
	int it = 0;
	long double fitness = INF;
	std::vector<int> Q(n);
	for (int i = 0; i < n; i++)
	{
		Q[i] = i;
	}
	while (it < MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA)
	{
		std::random_shuffle(Q.begin(), Q.end());
		for (int i = 0; i < n; i++)
		{
			long double overlapPenalty = bin_packing::getOverlapPenalty(packing, penalty, Q[i], 0, packing[Q[i]].outer()[0]);
			Point bestLocation(INF, INF);
			long double bestRotationAngle = 0.0;

			for (long double rotationAngle : allowableRoatations)
			{
				Point translationPoint = bin_packing::cuckooSearch(packing, penalty, Q[i], rotationAngle, width, length);
				long double currentOverlapPenalty = bin_packing::getOverlapPenalty(packing, penalty, Q[i], rotationAngle, translationPoint);
				if (currentOverlapPenalty < overlapPenalty)
				{
					overlapPenalty = currentOverlapPenalty;
					bestLocation = translationPoint;
					bestRotationAngle = rotationAngle;
				}
			}
			packing[Q[i]] = geo_util::rotatePolygon(packing[Q[i]], packing[Q[i]].outer()[0], bestRotationAngle);
			packing[Q[i]] = geo_util::translatePolygon(packing[Q[i]], bestLocation);
		}
		long double totalPenetrationDepth = bin_packing::getTotalPenetrationDepth(packing);
		if (geo_util::dblcmp(totalPenetrationDepth) == 0)
		{
			return packing;
		}
		else if (totalPenetrationDepth < fitness)
		{
			fitness = totalPenetrationDepth;
			it = 0;
		}

		bin_packing::increasePenalty(packing, penalty); // increase penalty
		it += 1;
	}
	return packing;
}
void bin_packing::cuckooPacking(MultiPolygon &initialPacking, long double runTimeDuration)
{
	auto start = std::chrono::high_resolution_clock::now();

	long double totalAreaOfInputPolygons = 0;
	for (Polygon &polygon : initialPacking)
	{
		totalAreaOfInputPolygons += std::fabs(boost_geo::area(polygon));
	}

	// MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
	MultiPolygon bestPacking = initialPacking;
	MultiPolygon currentPacking = initialPacking;

	long double bestLenght = geo_util::poly_util::getLength(initialPacking);
	long double decreasingRate = 0.10;
	long double increasingRate = 0.05;
	long double currentLength = (1.0 - decreasingRate) * bestLenght;

	while (true)
	{
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::minutes>(stop - start);
		if (duration.count() >= runTimeDuration)
		{
			break;
		}
		currentPacking = bin_packing::minimizeOverlap(currentPacking, ALLOWABLE_ROTATIONS, width, currentLength);
		if (bin_packing::isFeasible(currentPacking, totalAreaOfInputPolygons))
		{
			bestPacking = currentPacking;
			bestLenght = currentLength;
			currentLength = (1.0 - decreasingRate) * bestLenght;
		}
		else
		{
			currentLength = (1.0 + increasingRate) * bestLenght;
		}
	}
	geo_util::visualize(bestPacking, datasetName);
}

/** bin packing */
void bin_packing::binPacking(vector<Polygon> &polygons, long double width, string outputLocation, string datasetName, long double runTimeDuration)
{
	std::cout << "Calling binPacking..." << "...\n";
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

	std::cout << "Calling generateInitialSolution..." << "...\n";
	MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
	MultiPolygon bestPacking = initialPacking;
	std::cout << "generateInitialSolution completed..." << "...\n";

	// create a directory for initial solution and store the initial solution
	string initialSolutionDirectory = outputDirectoryName + "/initial_solution";
	mkdir(initialSolutionDirectory.c_str(), 0777);
	std::ofstream initialPackingWKTFile(initialSolutionDirectory + "/initial_packing.wkt");
	initialPackingWKTFile << boost_geo::wkt(initialPacking) << std::endl;
	initialPackingWKTFile.close();
	geo_util::visualize(initialPacking, initialSolutionDirectory, datasetName);
	std::cout << "Done..." << "...\n";


	double bestPackingArea = boost_geo::area(bestPacking);
	double bestLength = geo_util::poly_util::getLength(bestPacking);
    Box stock;
    boost_geo::envelope(bestPacking, stock);
    double stockArea = boost_geo::area(stock);
    Point stockDimension = stock.max_corner();
    boost_geo::subtract_point(stockDimension, stock.min_corner());

    std::cout << "\033[1;36mDataset....................: " << datasetName << "\n";
	std::cout << "\033[1;36mwidth......................: " << width << "\033[0m" << std::endl;
	std::cout << "\033[1;36mlength.....................: " << bestLength << "\033[0m" << std::endl;
    std::cout << "\033[1;36mtotal area of items........: " << totalAreaOfInputPolygons << "\033[0m" << std::endl;
    std::cout << "\033[1;36mresult polygon set area....: " << bestPackingArea << "\033[0m" << std::endl;
    std::cout << "\033[1;36mstock dimension [l * w]....: "
              << "[" << std::fabs(stockDimension.get<0>()) << ", " << std::fabs(stockDimension.get<1>()) << "]" << "\033[0m" << std::endl;
    std::cout << "\033[1;36mstock area.................: " << stockArea << "\033[0m" << std::endl;
    std::cout << "\033[1;32mpacking density............: " << (totalAreaOfInputPolygons / stockArea) * 100 << " %\033[0m" << std::endl;
}
