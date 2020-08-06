#include <bin_packing.hpp>

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
	for(Polygon polygon: multiPolygon)
	{
		for(Point point: polygon.outer())
		{
			packingLength = std::max(packingLength, point.get<1>());
			assert(point.get<1>() >= 0);
		}
	}
	return packingLength;
}

/** namespace polygon_fit **/
Polygon polygon_fit::getInnerFitRectangle(std::vector<Polygon> cluster, double length, double width)
{
	Polygon innerFitRectangle;
    Point reference = cluster[0].outer()[0];

	double max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
	for(Polygon &polygon: cluster)
	{
		for(Point &point: polygon.outer())
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
	innerFitRectangle.outer().push_back(Point(width - max_x,std::abs(min_y)));
	innerFitRectangle.outer().push_back(Point(std::abs(min_x), std::abs(min_y)));
	return innerFitRectangle;
}

Polygon polygon_fit::getNoFitPolygon(Polygon &polygon, std::vector<Polygon> cluster)
{
	Polygon noFitPolygon;
	return noFitPolygon;
}
std::vector<Polygon> polygon_fit::getAllNfpIfr(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> cluster)
{
	std::vector<Polygon> allNfpIfr;
	return allNfpIfr;
}
std::vector<Point> polygon_fit::getAllEdgeIntersectionPoints(std::vector<Polygon> &polygons)
{
	std::vector<Point> allEdgeIntersectionPoints;
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
std::vector<Point> cluster_util::getCandidatePlacementPositions(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> &clusterNextToBePlaced)
{
	std::vector<Point> candidatePlacementPositions;
	std::vector<Polygon> allNfpIfr = polygon_fit::getAllNfpIfr(alreadyPlacedPolygons, clusterNextToBePlaced);
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
Point cluster_util::findBlfPoint(std::vector<Polygon> &alreadyPlacedPolygons, std::vector<Polygon> &clusterNextToBePlaced)
{
	Point blfPoint(INF, INF);
	std::vector<Point> candidatePlacementPositions = cluster_util::getCandidatePlacementPositions(alreadyPlacedPolygons, clusterNextToBePlaced);
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
std::vector<Polygon> cluster_util::blf(std::vector<std::vector<Polygon>> &clusters)
{
	std::vector<Polygon> polygons;
	for (auto cluster : clusters)
	{
		Point point = cluster_util::findBlfPoint(polygons, cluster);
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

double cluster_util::getBestClusters(std::vector<std::vector<double>> &clusterValues, std::vector<double> &dp, int m, int mask)
{
	int taken = __builtin_popcount(mask);
	if (taken == (m + m))
		return 0.0;
	double &maxProfit = dp[mask];
	if (maxProfit != -1.0)
		return maxProfit;
	maxProfit = -INF;
	for (int i = 0; i < clusterValues.size(); i += 1)
	{
		int bit1 = mask & (1 << i);
		if (bit1 > 0)
			continue;
		for (int j = 0; j < clusterValues.size(); j += 1)
		{
			int bit2 = mask & (1 << j);
			if (bit2 > 0 or i == j)
				continue;
			int bitOn12 = (1 << i) + (1 << j);
			double __maxProfit = cluster_util::getBestClusters(clusterValues, dp, m, mask ^ bitOn12) + clusterValues[i][j];
			maxProfit = std::max(maxProfit, __maxProfit);
		}
	}
	return maxProfit;
}
void cluster_util::printBestClusters(std::vector<std::vector<double>> &clusterValues, std::vector<double> &dp, int m, int mask, std::vector<std::tuple<int, int>> &pairs)
{
	int taken = __builtin_popcount(mask);
	if (taken == (m + m))
		return;
	double &maxProfit = dp[mask];
	for (int i = 0; i < clusterValues.size(); i += 1)
	{
		int bit1 = mask & (1 << i);
		if (bit1 > 0)
			continue;
		for (int j = 0; j < clusterValues.size(); j += 1)
		{
			int bit2 = mask & (1 << j);
			if (bit2 > 0 or i == j)
				continue;
			int bitOn12 = (1 << i) + (1 << j);
			double __maxProfit = cluster_util::getBestClusters(clusterValues, dp, m, mask ^ bitOn12) + clusterValues[i][j];
			if (maxProfit == __maxProfit)
			{
				pairs.push_back({i, j});
				cluster_util::printBestClusters(clusterValues, dp, m, mask ^ bitOn12, pairs);
				return;
			}
		}
	}
}
std::vector<std::tuple<int, int>> cluster_util::perfectClustering(std::vector<std::vector<double>> &clusterValues, double noOfclusterPairs)
{
	std::vector<std::tuple<int, int>> clusterIds;
	std::vector<double> dp(1 << 22, -1.0);
	double bestClusterizationValue = cluster_util::getBestClusters(clusterValues, dp, noOfclusterPairs, 0);
	cluster_util::printBestClusters(clusterValues, dp, noOfclusterPairs, 0, clusterIds);
	return clusterIds;
}
std::vector<std::vector<double>> cluster_util::getClusterValues(std::vector<Polygon> &polygons)
{
	int n = polygons.size();
	std::vector<std::vector<double>> clusterValues(n, std::vector<double>(n, 0.0));
	for (int i = 0; i < n; i += 1)
	{
		for (int j = 0; j < n; j += 1)
		{
			if (i != j)
			{
				Polygon nfp = polygon_fit::getNoFitPolygon(polygons[i], {polygons[j]});
				if (geo_util::isConcave(nfp) == true)
				{
					Point point = cluster_util::findDominantPoint(nfp);
					Polygon polygon_j = geo_util::translatePolygon(polygons[j], point);
					clusterValues[i][j] = cluster_util::getClusterValue(polygons[i], polygon_j);
				}
			}
		}
	}

	// @validation
	for (int i = 0; i < n; i += 1)
	{
		for (int j = 0; j < n; j += 1)
		{
			assert(!(clusterValues[i][j] < 0.0) and !(clusterValues[i][j] > 2.0));
		}
	}
	return clusterValues;
}

MultiPolygon cluster_util::generateInitialSolution(std::vector<Polygon> &polygons, double width)
{
	std::vector<std::vector<double>> clusterValues = cluster_util::getClusterValues(polygons);
	std::vector<std::tuple<int, int>> clusterIds = cluster_util::perfectClustering(clusterValues, std::min((int)polygons.size(), 10));

	std::vector<bool> taken((int)polygons.size(), false);
	std::vector<std::vector<Polygon>> clusters;
	for (std::tuple<int, int> clusterId : clusterIds)
	{
		int i, j;
		std::tie(i, j) = clusterId;
		taken[i] = taken[j] = true;
		std::vector<Polygon> cluster;
		cluster.push_back(polygons[i]);
		cluster.push_back(polygons[j]);
		clusters.push_back(cluster);
	}
	cluster_util::sort(clusters);
	std::vector<Polygon> initialSolution = cluster_util::blf(clusters);

	// @validation
	int n = polygons.size();
	int m = 0;
	for (auto cluster : clusters)
	{
		m += (int)cluster.size();
	}
	assert(
		clusterValues.size() == n and
		clusterValues[0].size() == n and
		m == n and
		initialSolution.size() == n);

	MultiPolygon result;
	for (Polygon &polygon : initialSolution)
	{
		result.push_back(polygon);
	}
	return result;
}
/* namespace bin_packing */
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
bool bin_packing::isFeasible(MultiPolygon &packing, double totalAreaOfInputPolygons)
{
	double overlappingArea = 0.0;
	double feasibilityRatio = 0.0;
	int n = packing.size();
	for(int i = 0; i < n; i += 1) 
	{
		for(int j = i + 1; j < n; j += 1)
		{
			overlappingArea += geo_util::polygonPolygonIntersectionArea(packing[i], packing[j]);
		}
	}
	feasibilityRatio = overlappingArea / totalAreaOfInputPolygons;
	return geo_util::dblcmp(feasibilityRatio - FEASIBILTY, EPS) >= 0 ? true : false;
}
MultiPolygon bin_packing::minimizeOverlap(MultiPolygon &multiPolygon, std::vector<double> &allowableRoatations, double width, double length)
{
	MultiPolygon packing;
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
		currentPacking;// = bin_packing::minimizeOverlap(currentPacking, ALLOWABLE_ROTATIONS, width, currentLength);
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
	// visualize
}
