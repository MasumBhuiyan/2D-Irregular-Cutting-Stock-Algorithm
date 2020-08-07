#include <bin_packing.hpp>
#include <libnfporb.hpp>

using namespace libnfporb;
/** namespace geo_util **/
int geo_util::dblcmp(double d, double eps)
{
	if (std::fabs(d) < eps)
	{
		return 0;
	}
	return d > eps ? 1 : -1;
}

bool geo_util::isConcave(Polygon &polygon)
{
	int n = polygon.outer().size();
	if (n < 4)
		return true;

	bool sign = false;
	for (int i = 0; i < n; i += 1)
	{
		double dx1 = polygon.outer()[(i + 2) % n].get<0>() - polygon.outer()[(i + 1) % n].get<0>();
		double dy1 = polygon.outer()[(i + 2) % n].get<1>() - polygon.outer()[(i + 1) % n].get<1>();
		double dx2 = polygon.outer()[i].get<0>() - polygon.outer()[(i + 1) % n].get<0>();
		double dy2 = polygon.outer()[i].get<1>() - polygon.outer()[(i + 1) % n].get<1>();
		double zcrossproduct = dx1 * dy2 - dy1 * dx2;
		if (i == 0)
			sign = zcrossproduct > 0;
		else if (sign != (zcrossproduct > 0))
			return true;
	}

	return false;
}

double geo_util::getLength(Polygon &polygon)
{
	double min_x = INF, max_x = -INF;
	for (auto point : polygon.outer())
	{
		min_x = std::min(point.get<0>(), min_x);
		max_x = std::max(point.get<0>(), max_x);
	}
	return max_x - min_x;
}

double geo_util::getWidth(Polygon &polygon)
{
	double min_y = INF, max_y = -INF;
	for (auto point : polygon.outer())
	{
		min_y = std::min(point.get<1>(), min_y);
		max_y = std::max(point.get<1>(), max_y);
	}
	return max_y - min_y;
}

double geo_util::crossProduct(Point p, Point q)
{
	return p.get<0>() * q.get<1>() - p.get<1>() * q.get<0>();
}

void geo_util::normalize(Polygon &polygon)
{
	Point reference = polygon.outer()[0];
	for (auto &point : polygon.outer())
	{
		point = Point(point.get<0>() - reference.get<0>(), point.get<1>() - reference.get<1>());
	}
}

int geo_util::orientation(Point a, Point b, Point c)
{
	double x1 = b.get<0>(), y1 = b.get<1>();
	double x2 = c.get<0>(), y2 = c.get<1>();
	double _x = a.get<0>(), _y = a.get<1>();
	double p = crossProduct(Point(x1 - _x, y1 - _y), Point(x2 - _x, y2 - _y));
	if (p == 0)
		return 0;
	return p < 0 ? -1 : 1;
}

Polygon geo_util::rotatePolygon(Polygon polygon, Point reference, double degree)
{
	Polygon rotatedPolygon;
	for (auto &point : polygon.outer())
	{
		Point t(point.get<0>() - reference.get<0>(), point.get<1>() - reference.get<1>());
		point = t;
	}
	boost_geo::transform(polygon, rotatedPolygon, trans::rotate_transformer<boost_geo::degree, double, 2, 2>(degree));
	for (auto &point : rotatedPolygon.outer())
	{
		Point t(point.get<0>() + reference.get<0>(), point.get<1>() + reference.get<1>());
		point = t;
	}
	return rotatedPolygon;
}

Polygon geo_util::translatePolygon(Polygon polygon, Point point)
{
	Polygon translatedPolygon;
	Point reference = polygon.outer()[0];
	for (auto &point : polygon.outer())
	{
		Point t(point.get<0>() - reference.get<0>(), point.get<1>() - reference.get<1>());
		point = t;
	}
	boost_geo::transform(polygon, translatedPolygon, trans::translate_transformer<double, 2, 2>(point.get<0>(), point.get<1>()));
	return translatedPolygon;
}

double geo_util::linePointDistance(Point a, Point b, Point p)
{
	double x1 = a.get<0>(), y1 = a.get<1>();
	double x2 = b.get<0>(), y2 = b.get<1>();
	double _a = y1 - y2, _b = x2 - x1, _c = x1 * y2 - x2 * y1;
	double distance = std::fabs(_a * p.get<0>() + _b * p.get<1>() + _c) / sqrt(_a * _a + _b * _b);
	return distance;
}

double geo_util::polygonPolygonIntersectionArea(Polygon &polygon1, Polygon &polygon2)
{
	std::deque<Polygon> polygons;
	boost_geo::intersection(polygon1, polygon2, polygons);
	double intersectedArea = 0.0;
	for (auto polygon : polygons)
	{
		intersectedArea += boost_geo::area(polygon);
	}
	return std::fabs(intersectedArea);
}

Point geo_util::segmentSegmentIntersectionPoint(Point a, Point b, Point c, Point d)
{
	int o1 = geo_util::orientation(a, b, c), o2 = geo_util::orientation(a, b, d);
	int o3 = geo_util::orientation(c, d, a), o4 = geo_util::orientation(c, d, b);

	if ((o1 * o2 == -1 && o3 * o4 == -1) || (!o1 && geo_util::pointInRectangle(a, b, c)) || (!o2 && geo_util::pointInRectangle(a, b, d)) || (!o3 && geo_util::pointInRectangle(c, d, a)) || (!o4 && geo_util::pointInRectangle(c, d, b)))
	{
		double a1 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(a.get<0>() - c.get<0>(), a.get<1>() - c.get<1>()));
		double a2 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(b.get<0>() - d.get<0>(), b.get<1>() - d.get<1>()));
		return Point((a.get<0>() * a2 - b.get<0>() * a1) / (a2 - a1), (a.get<1>() * a2 - b.get<1>() * a1) / (a2 - a1));
	}
	return Point(INF, INF);
}

std::vector<Polygon> geo_util::rotatePolygons(std::vector<Polygon> &polygons, Point reference, double degree)
{
	std::vector<Polygon> rotatedPolygons;
	for (auto &polygon : polygons)
	{
		rotatedPolygons.push_back(geo_util::rotatePolygon(polygon, reference, degree));
	}
	return rotatedPolygons;
}

std::vector<Polygon> geo_util::translatePolygons(std::vector<Polygon> polygons, Point point)
{
	std::vector<Polygon> translatedPolygons;
	Point reference = polygons[0].outer()[0];

	for (auto &polygon : polygons)
	{
		for (auto &point : polygon.outer())
		{
			Point t(point.get<0>() - reference.get<0>(), point.get<1>() - reference.get<1>());
			point = t;
		}
	}
	for (auto &polygon : polygons)
	{
		Polygon translatedPolygon;
		boost_geo::transform(polygon, translatedPolygon, trans::translate_transformer<double, 2, 2>(point.get<0>(), point.get<1>()));
		translatedPolygons.push_back(translatedPolygon);
	}
	return translatedPolygons;
}

bool geo_util::isItPossibleToPlacePolygon(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> clusterNextToBePlaced, Point point)
{
	clusterNextToBePlaced = geo_util::translatePolygons(clusterNextToBePlaced, point);
	for (auto poly : alreadyPlacedPolygons)
	{
		for (auto polygonNextToBePlaced : clusterNextToBePlaced)
		{
			if (geo_util::polygonPolygonIntersectionArea(polygonNextToBePlaced, poly) > 0)
				return false;
		}
	}
	return true;
}

bool geo_util::pointInRectangle(Point a, Point b, Point c)
{
	bool p = (std::min(a.get<0>(), b.get<0>()) <= c.get<0>() && std::max(a.get<0>(), b.get<0>()) >= c.get<0>());
	bool q = (std::min(a.get<1>(), b.get<1>()) <= c.get<1>() && std::max(a.get<1>(), b.get<1>()) >= c.get<1>());
	return (p && q);
}

double geo_util::getPackingLength(MultiPolygon &multiPolygon)
{
	double packingLength = 0.0;
	for (Polygon polygon : multiPolygon)
	{
		for (Point point : polygon.outer())
		{
			packingLength = std::max(packingLength, point.get<1>());
			assert(point.get<1>() >= 0);
		}
	}
	return packingLength;
}
Polygon geo_util::makePolygon(Polygon polygon, Point translationPoint, double rotationAngle)
{
	polygon = geo_util::rotatePolygon(polygon, polygon.outer()[0], rotationAngle);
	polygon = geo_util::translatePolygon(polygon, translationPoint);
	return polygon;
}
void geo_util::visualize(MultiPolygon &multipolygon, std::string datasetName)
{
    Box box;
    boost::geometry::envelope(multipolygon, box);
    std::cout << "make_envelope..............: " << boost::geometry::dsv(box) << std::endl;
    std::ostringstream name;
    name << "frame_" << std::setw(4) << std::setfill('0') << frameno++ << "_" << datasetName << ".svg";
    std::ofstream svg(name.str());
    boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
    mapper.add(multipolygon);
    mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(204,153,0);stroke:rgb(204,153,0);stroke-width:1", 5);
    mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(255,128,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
}
/** namespace polygon_fit **/
Polygon polygon_fit::getInnerFitRectangle(std::vector<Polygon> cluster, double length, double width)
{
	Polygon innerFitRectangle;
	Point reference = cluster[0].outer()[0];

	double max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
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

Polygon polygon_fit::getNoFitPolygon(Polygon &polygon, std::vector<Polygon> cluster)
{
	Polygon noFitPolygon;
	polygon_t polygon1, polygon2;
	for (auto point : polygon.outer()) 
	{
		polygon1.outer().push_back(point_t(point.get<0>(), point.get<1>()));
	}
	for (auto point : cluster[0].outer()) 
	{
		polygon2.outer().push_back(point_t(point.get<0>(), point.get<1>()));
	}
	MultiPolygon multiPolygon;
	nfp_t nfp = generateNFP(polygon1, polygon2, true);
	for(auto poly : nfp) 
	{
		Polygon ring;
		for (auto point : poly) {
			Point t((double) point.x_.val(), (double) point.y_.val());
			ring.outer().push_back(t);
		}
		multiPolygon.push_back(ring);
	}
	return multiPolygon[0];
}
MultiPolygon polygon_fit::getAllNfpIfr(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> cluster, double length, double width)
{
	MultiPolygon allNfpIfr;
	Polygon ifr = polygon_fit::getInnerFitRectangle(cluster, length, width);
	allNfpIfr.push_back(ifr);
	for(auto polygon: alreadyPlacedPolygons)
	{
		auto nfp = polygon_fit::getNoFitPolygon(polygon, cluster);
		allNfpIfr.push_back(nfp);
	}
	return allNfpIfr;
}
std::vector<Point> polygon_fit::getAllEdgeIntersectionPoints(std::vector<Polygon> &polygons)
{
	std::vector<Point> allEdgeIntersectionPoints;
	std::vector<std::tuple<Point,Point>> edges;

	for(auto polygon: polygons)
	{
		for(int i = 0; i + 1 < polygon.outer().size(); i += 1)
		{
			edges.push_back({polygon.outer()[i], polygon.outer()[i + 1]});
		}
	}
	for(int i = 0; i < edges.size(); i += 1)
	{
		for(int j = i + 1; j < edges.size(); j += 1)
		{
			Point point1, point2, point3, point4;
			std::tie(point1, point2) = edges[i];
			std::tie(point3, point4) = edges[j];
			Point inetersectionPoint = geo_util::segmentSegmentIntersectionPoint(point1, point2, point3, point4);
			if( inetersectionPoint.get<0>() == INF and inetersectionPoint.get<1>() == INF ) continue;
			allEdgeIntersectionPoints.push_back(inetersectionPoint);	
		}
	}
	return allEdgeIntersectionPoints;
}

/** namespace cluster_util **/
Polygon cluster_util::convexHull(MultiPolygon multiPolygon)
{
	Polygon convexhull;
	boost_geo::convex_hull(multiPolygon, convexhull);
	return convexhull;
}
std::vector<Polygon> cluster_util::findConvexHullVacancy(Polygon &polygon)
{
	std::vector<Polygon> vacancies;
	Polygon convexhull = cluster_util::convexHull({polygon});
	boost_geo::difference(convexhull, polygon, vacancies);
	return vacancies;
}
std::vector<std::vector<Point>> cluster_util::findOppositeSideOfVacancies(Polygon &convexhull, std::vector<Polygon> &vacancies)
{
	std::vector<std::vector<Point>> oppositeSideOfVacancies;
	for (auto vacancy : vacancies)
	{
		std::vector<Point> line;
		assert(boost_geo::intersection(convexhull, vacancy, line) == true);
		oppositeSideOfVacancies.push_back(line);
	}
	return oppositeSideOfVacancies;
}
Point cluster_util::findDominantPoint(Polygon &polygon)
{
	Point dominant;
	Polygon convexhull = cluster_util::convexHull({polygon});
	std::vector<Polygon> vacancies = cluster_util::findConvexHullVacancy(polygon);
	std::vector<std::vector<Point>> oppositeSideOfVacancies = cluster_util::findOppositeSideOfVacancies(convexhull, vacancies);

	int n = oppositeSideOfVacancies.size();
	assert(vacancies.size() == oppositeSideOfVacancies.size());
	double maxDistance = 0.0;

	for (int i = 0; i < n; i += 1)
	{
		for (Point point : vacancies[i].outer())
		{
			double distance = geo_util::linePointDistance(oppositeSideOfVacancies[i][0], oppositeSideOfVacancies[i][1], point);
			if (distance > maxDistance)
			{
				maxDistance = distance;
				dominant = point;
			}
		}
	}
	return dominant;
}
std::vector<std::vector<Point>> cluster_util::findAllPairDominantPoint(std::vector<std::vector<Polygon>> &noFitPolygons)
{
	int n = noFitPolygons.size();
	std::vector<std::vector<Point>> points(n, std::vector<Point>(n));
	for (int i = 0; i < n; i += 1)
	{
		for (int j = 0; j < n; j += 1)
		{
			if (i != j)
			{
				points[i][j] = cluster_util::findDominantPoint(noFitPolygons[i][j]);
			}
		}
	}
	return points;
}
std::vector<std::vector<Polygon>> cluster_util::findAllConvexHullVacancies(std::vector<Polygon> &polygons)
{
	std::vector<std::vector<Polygon>> allConvexHullVacancies;
	for (auto &polygon : polygons)
	{
		allConvexHullVacancies.push_back(cluster_util::findConvexHullVacancy(polygon));
	}
	return allConvexHullVacancies;
}
double cluster_util::clusteringCriteria1(Polygon &polygon1, Polygon &polygon2)
{
	double value = 0.0, value1 = 0.0, value2 = 0.0;
	double areaIntersection1 = 0.0, totalVacancy1 = 0.0;
	double areaIntersection2 = 0.0, totalVacancy2 = 0.0;

	std::vector<Polygon> V1 = cluster_util::findConvexHullVacancy(polygon1);
	std::vector<Polygon> V2 = cluster_util::findConvexHullVacancy(polygon2);

	for (auto vacancy : V2)
	{
		double area = geo_util::polygonPolygonIntersectionArea(polygon1, vacancy);
		if (area > 0)
		{
			areaIntersection1 += area;
			totalVacancy1 += std::fabs(boost_geo::area(vacancy));
		}
	}
	for (auto vacancy : V1)
	{
		double area = geo_util::polygonPolygonIntersectionArea(polygon2, vacancy);
		if (area > 0)
		{
			areaIntersection2 += area;
			totalVacancy2 += std::fabs(boost_geo::area(vacancy));
		}
	}
	if (totalVacancy1 > 0)
		value1 = areaIntersection1 / totalVacancy1;
	if (totalVacancy2 > 0)
		value2 = areaIntersection2 / totalVacancy2;
	value = std::max(value1, value2);
	return value;
}
double cluster_util::clusteringCriteria2(Polygon &polygon1, Polygon &polygon2)
{
	MultiPolygon multiPolygon;
	multiPolygon.push_back(polygon1);
	multiPolygon.push_back(polygon2);
	Polygon convexhull = cluster_util::convexHull(multiPolygon);

	double area1 = std::fabs(boost_geo::area(polygon1));
	double area2 = std::fabs(boost_geo::area(polygon2));
	double area3 = std::fabs(boost_geo::area(convexhull));
	assert(area3 > 0);

	double value = (area1 + area2) / area3;
	return value;
}
double cluster_util::getClusterValue(Polygon &polygon1, Polygon &polygon2)
{
	double value = cluster_util::clusteringCriteria1(polygon1, polygon2) + cluster_util::clusteringCriteria2(polygon1, polygon2);
	return value;
}

void cluster_util::sort(std::vector<std::vector<Polygon>> &clusters)
{
	sort(clusters.begin(), clusters.end(), [](std::vector<Polygon> cluster1, std::vector<Polygon> cluster2) {
		double area1 = 0.0, area2 = 0.0;
		for (auto polygon : cluster1)
			area1 += std::fabs(boost_geo::area(polygon));
		for (auto polygon : cluster2)
			area2 += std::fabs(boost_geo::area(polygon));
		return geo_util::dblcmp(area1 - area2, EPS) >= 0;
	});
}
std::vector<Point> cluster_util::getCandidatePlacementPositions(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> &clusterNextToBePlaced, double length, double width)
{
	std::vector<Point> candidatePlacementPositions;
	auto allNfpIfr = polygon_fit::getAllNfpIfr(alreadyPlacedPolygons, clusterNextToBePlaced, length, width);
	std::vector<Point> allEdgeIntersectionPoints = polygon_fit::getAllEdgeIntersectionPoints(allNfpIfr);
	for (auto polygon : allNfpIfr)
	{
		for (auto point : polygon.outer())
		{
			candidatePlacementPositions.push_back(point);
		}
	}
	for (auto point : allEdgeIntersectionPoints)
	{
		candidatePlacementPositions.push_back(point);
	}
	return candidatePlacementPositions;
}
Point cluster_util::findBlfPoint(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> &clusterNextToBePlaced, double length, double width)
{
	Point blfPoint(INF, INF);
	std::vector<Point> candidatePlacementPositions = cluster_util::getCandidatePlacementPositions(alreadyPlacedPolygons, clusterNextToBePlaced, length, width);
	for (auto point : candidatePlacementPositions)
	{
		if (geo_util::isItPossibleToPlacePolygon(alreadyPlacedPolygons, clusterNextToBePlaced, point) == true)
		{
			if (point.get<0>() < blfPoint.get<0>())
			{
				blfPoint = point;
			}
			else if (point.get<0>() == blfPoint.get<0>() and point.get<1>() < blfPoint.get<1>())
			{
				blfPoint = point;
			}
		}
	}

	// @validation
	assert(blfPoint.get<0>() != INF and blfPoint.get<1>() != INF);
	return blfPoint;
}
std::vector<Polygon> cluster_util::blf(std::vector<std::vector<Polygon>> &clusters, double length, double width)
{
	std::vector<Polygon> polygons;
	for (auto cluster : clusters)
	{
		Point point = cluster_util::findBlfPoint(polygons, cluster,length, width);
		std::vector<Polygon> translatedCluster = geo_util::translatePolygons(cluster, point);
		for (auto polygon : translatedCluster)
		{
			polygons.push_back(polygon);
		}
	}

	// @validation
	int m = 0;
	for (auto cluster : clusters)
	{
		m += (int)cluster.size();
	}
	assert(polygons.size() == m);

	for (int i = 0; i < m; i += 1)
	{
		for (int j = i + 1; j < m; j += 1)
		{
			assert(!(geo_util::polygonPolygonIntersectionArea(polygons[i], polygons[j]) > 0));
		}
	}
	return polygons;
}

double cluster_util::getBestClusters(std::vector<std::vector<std::vector<std::vector<double>>>> &clusterValues, std::vector<double> &dp, int m, int mask)
{
	int taken = __builtin_popcount(mask);
	if (taken == (m + m))
		return 0.0;
	double &maxProfit = dp[mask];
	if (maxProfit != -1.0)
		return maxProfit;
	maxProfit = -INF;

	int POLYGON_SIZE = clusterValues[0].size();
	for (int i = 0; i < POLYGON_SIZE; i += 1)
	{
		int bit1 = mask & (1 << i);
		if (bit1 > 0)
			continue;
		for (int j = 0; j < POLYGON_SIZE; j += 1)
		{
			int bit2 = mask & (1 << j);
			if (bit2 > 0 or i == j)
				continue;

			for(int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
			{
				for(int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
				{
					int bitOn12 = (1 << i) + (1 << j);
					double __maxProfit = cluster_util::getBestClusters(clusterValues, dp, m, mask ^ bitOn12) + clusterValues[i][j][k][l];
					maxProfit = std::max(maxProfit, __maxProfit);
				}
			}
		}
	}
	return maxProfit;
}
void cluster_util::printBestClusters(std::vector<std::vector<std::vector<std::vector<double>>>> &clusterValues, std::vector<double> &dp, int m, int mask, std::vector<std::tuple<int, int, int, int>> &pairs)
{
	int taken = __builtin_popcount(mask);
	if (taken == (m + m))
		return;
	double &maxProfit = dp[mask];
	int POLYGON_SIZE = clusterValues[0].size();
	for (int i = 0; i < POLYGON_SIZE; i += 1)
	{
		int bit1 = mask & (1 << i);
		if (bit1 > 0)
			continue;
		for (int j = 0; j < POLYGON_SIZE; j += 1)
		{
			int bit2 = mask & (1 << j);
			if (bit2 > 0 or i == j)
				continue;
			for(int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
			{
				for(int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
				{
					int bitOn12 = (1 << i) + (1 << j);
					double __maxProfit = cluster_util::getBestClusters(clusterValues, dp, m, mask ^ bitOn12) + clusterValues[i][j][k][l];
					if (maxProfit == __maxProfit)
					{
						pairs.push_back({i, j, k, l});
						cluster_util::printBestClusters(clusterValues, dp, m, mask ^ bitOn12, pairs);
						return;
					}
				}
			}
		}
	}
}
std::vector<std::tuple<int, int, int, int>> cluster_util::perfectClustering(std::vector<std::vector<std::vector<std::vector<double>>>> &clusterValues, double noOfclusterPairs)
{
	std::vector<std::tuple<int, int, int, int>> clusterIds;
	std::vector<double> dp(1 << 22, -1.0);
	double bestClusterizationValue = cluster_util::getBestClusters(clusterValues, dp, noOfclusterPairs, 0);
	cluster_util::printBestClusters(clusterValues, dp, noOfclusterPairs, 0, clusterIds);
	return clusterIds;
}
std::vector<std::vector<std::vector<std::vector<double>>>> cluster_util::getClusterValues(std::vector<Polygon> &polygons)
{
	int n = polygons.size();
	std::vector<std::vector<std::vector<std::vector<double>>>> clusterValues;
	clusterValues.resize(n, std::vector<std::vector<std::vector<double>>>(n, std::vector<std::vector<double>>(ALLOWABLE_ROTATIONS.size(), std::vector<double>(ALLOWABLE_ROTATIONS.size(), 0.0))));
	for (int i = 0; i < n; i += 1)
	{
		for (int j = 0; j < n; j += 1)
		{
			if (i != j)
			{
				for(int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
				{
					for(int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
					{
						Polygon polygon_i = geo_util::makePolygon(polygons[i], polygons[i].outer()[0], ALLOWABLE_ROTATIONS[k]);
						Polygon polygon_j = geo_util::makePolygon(polygons[j], polygons[j].outer()[0], ALLOWABLE_ROTATIONS[l]);
						auto nfp = polygon_fit::getNoFitPolygon( polygon_i, {polygon_j});
						if (geo_util::isConcave(nfp) == true)
						{
							Point point = cluster_util::findDominantPoint(nfp);
							polygon_j = geo_util::translatePolygon(polygon_j, point);
							clusterValues[i][j][k][l] = cluster_util::getClusterValue(polygon_i, polygon_j);
						}
					}
				}
			}
		}
	}
	return clusterValues;
}

MultiPolygon cluster_util::generateInitialSolution(std::vector<Polygon> &polygons, double width)
{
	std::vector<std::vector<std::vector<std::vector<double>>>> clusterValues = cluster_util::getClusterValues(polygons);
	std::vector<std::tuple<int, int, int, int>> clusterIds = cluster_util::perfectClustering(clusterValues, std::min((int)polygons.size(), 10));

	std::vector<bool> taken((int)polygons.size(), false);
	std::vector<std::vector<Polygon>> clusters;
	for (std::tuple<int, int, int, int> clusterId : clusterIds)
	{
		int i, j, k, l;
		std::tie(i, j, k, l) = clusterId;
		std::vector<Polygon> cluster;
		
		Polygon polygon_i = geo_util::makePolygon(polygons[i], polygons[i].outer()[0], ALLOWABLE_ROTATIONS[k]);
		Polygon polygon_j = geo_util::makePolygon(polygons[j], polygons[j].outer()[0], ALLOWABLE_ROTATIONS[l]);
		auto nfp = polygon_fit::getNoFitPolygon( polygon_i, {polygon_j});
		if (geo_util::isConcave(nfp) == true)
		{
			taken[i] = taken[j] = true;
			Point point = cluster_util::findDominantPoint(nfp);
			polygon_j = geo_util::translatePolygon(polygon_j, point);
			cluster.push_back(polygon_i);
			cluster.push_back(polygon_j);
	     	clusters.push_back(cluster);
		}
	}

	for(auto polygon: polygons)
	{
		clusters.push_back({polygon});
	}
	cluster_util::sort(clusters);
	double length = 100000;
	std::vector<Polygon> initialSolution = cluster_util::blf(clusters, length, width);
	MultiPolygon result;
	for (Polygon &polygon : initialSolution)
	{
		result.push_back(polygon);
	}
	return result;
}

/** namespace bin_packing */
/** 
 * read input from dataset file
 * file location ../io/datasets/**.txt
 */
std::tuple<std::vector<Polygon>, double> bin_packing::readDataset(std::string datasetName)
{
	double width;
	std::vector<Polygon> polygons;
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
		Polygon polygon;
		for (int i = 0; i < n; i += 1)
		{
			int quantity;
			file >> quantity;
			int numberOfVirtices;
			file >> numberOfVirtices;
			for (int j = 0; j < numberOfVirtices; j++)
			{
				double _x, _y;
				file >> _x >> _y;
				polygon.outer().push_back(Point(_x, _y));
			}
			polygon.outer().push_back(polygon.outer()[0]);
			reverse(polygon.outer().begin(), polygon.outer().end());
			while (quantity--)
			{
				polygons.push_back(polygon);
			}
		}
	}
	file.close();
	return {polygons, width};
}
/**
 * checks feasibilty of a packing
 * compares with FEASIBILTY limit
 */
bool bin_packing::isFeasible(MultiPolygon &packing, double totalAreaOfInputPolygons)
{
	double overlappingArea = 0.0;
	double feasibilityRatio = 0.0;
	int n = packing.size();
	for (int i = 0; i < n; i += 1)
	{
		for (int j = i + 1; j < n; j += 1)
		{
			overlappingArea += geo_util::polygonPolygonIntersectionArea(packing[i], packing[j]);
		}
	}
	feasibilityRatio = overlappingArea / totalAreaOfInputPolygons;
	return geo_util::dblcmp(feasibilityRatio - FEASIBILTY, EPS) >= 0 ? true : false;
}
/**
 * naive implementation
 * may be changed in future
 */
double bin_packing::getPenetrationDepth(Polygon polygonA, Polygon polygonB)
{
	MultiPolygon intersections;
	boost_geo::intersection(polygonA, polygonB, intersections);
	return std::fabs(boost_geo::area(intersections));
}
/**
 * returns total penitration depth of a pcking
 */
double bin_packing::getTotalPenetrationDepth(MultiPolygon &packing)
{
	double totalPenetrationDepth = 0.0;
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
double bin_packing::getOverlapPenalty(MultiPolygon &packing, std::vector<std::vector<double>> &penalty, int id, double rotationAngle, Point translationPoint)
{
	int n = packing.size();
	Polygon polygon = packing[id];
	polygon = geo_util::rotatePolygon(polygon, polygon.outer()[0], rotationAngle);
	polygon = geo_util::translatePolygon(polygon, translationPoint);

	double overlapPenalty = 0;
	for (int j = 0; j < n; j++)
	{
		overlapPenalty += (penalty[id][j] * bin_packing::getPenetrationDepth(polygon, packing[j]));
	}
	return overlapPenalty;
}
void bin_packing::increasePenalty(MultiPolygon &packing, std::vector<std::vector<double>> &penalty)
{
	int n = penalty.size();
	double maximumPenetrationDepth = -INF;
	std::vector<std::vector<double>> penetrationDepths(n, std::vector<double>(n, 0));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
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
Point bin_packing::cuckooSearch(MultiPolygon &packing, std::vector<std::vector<double>> &penalty, int id, double rotationAngle, double width, double lenght)
{
	Polygon polygon = packing[id];
	polygon = geo_util::rotatePolygon(polygon, polygon.outer()[0], rotationAngle);
	Polygon innerFitRectangle = polygon_fit::getInnerFitRectangle({polygon}, lenght, width);
	assert(innerFitRectangle.outer().size() == 5);
	double max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
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
		double overlapPenalty_i = bin_packing::getOverlapPenalty(packing, penalty, id, rotationAngle, hostNests[i]);

		srand(time(NULL));
		int j = std::rand() % NUMBER_OF_HOST_NESTS;
		double overlapPenalty_j = bin_packing::getOverlapPenalty(packing, penalty, id, rotationAngle, hostNests[j]);

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
MultiPolygon bin_packing::minimizeOverlap(MultiPolygon packing, std::vector<double> allowableRoatations, double width, double length)
{
	int n = packing.size();
	std::vector<std::vector<double>> penalty(n, std::vector<double>(n, 1.0));
	int it = 0;
	double fitness = INF;
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
			double overlapPenalty = bin_packing::getOverlapPenalty(packing, penalty, Q[i], 0, packing[Q[i]].outer()[0]);
			Point bestLocation(INF, INF);
			double bestRotationAngle = 0.0;

			for (double rotationAngle : allowableRoatations)
			{
				Point translationPoint = bin_packing::cuckooSearch(packing, penalty, Q[i], rotationAngle, width, length);
				double currentOverlapPenalty = bin_packing::getOverlapPenalty(packing, penalty, Q[i], rotationAngle, translationPoint);
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
		double totalPenetrationDepth = bin_packing::getTotalPenetrationDepth(packing);
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
void bin_packing::binPacking(std::vector<Polygon> &polygons, double width, std::string location, std::string datasetName, double runTimeDuration)
{
	auto start = std::chrono::high_resolution_clock::now();

	double totalAreaOfInputPolygons = 0;
	for (Polygon &polygon : polygons)
	{
		totalAreaOfInputPolygons += std::fabs(boost_geo::area(polygon));
	}

	MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
	MultiPolygon bestPacking = initialPacking;
	MultiPolygon currentPacking = initialPacking;

	double bestLenght = geo_util::getPackingLength(initialPacking);
	double decreasingRate = 0.1;
	double increasingRate = 0.1;
	double currentLength = (1.0 - decreasingRate) * bestLenght;

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
