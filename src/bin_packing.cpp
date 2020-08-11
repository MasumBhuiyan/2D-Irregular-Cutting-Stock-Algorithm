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

	if ((o1 * o2 == -1 && o3 * o4 == -1) || (!o1 && geo_util::pointInRectangle(a, b, c)) || (!o2 && geo_util::pointInRectangle(a, b, d)) || (!o3 && geo_util::pointInRectangle(c, d, a)) || (!o4 && geo_util::pointInRectangle(c, d, b)))
	{
		double a1 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(a.get<0>() - c.get<0>(), a.get<1>() - c.get<1>()));
		double a2 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(b.get<0>() - d.get<0>(), b.get<1>() - d.get<1>()));
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

Polygon geo_util::poly_util::rotateCW(Polygon &polygon, long double rotationAngleInDegree, Point referencePoint)
{
	boost_geo::multiply_value(referencePoint, -1);
	Polygon translatedPolygon = geo_util::poly_util::translate(polygon, referencePoint), rotatedPolygon;
	boost_geo::transform(translatedPolygon, rotatedPolygon, trans::rotate_transformer<boost_geo::degree, long double, 2, 2>(rotationAngleInDegree));
	return rotatedPolygon;
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
	Point referecenPoint = clusterNextToBePlaced.front().outer().front();
	boost_geo::multiply_value(referecenPoint, -1);
	for (auto &polygon : clusterNextToBePlaced)
	{
		polygon = geo_util::poly_util::translate(polygon, referecenPoint);
		polygon = geo_util::poly_util::translate(polygon, point);
	}
	for (auto poly : packing)
	{
		for (auto polygonNextToBePlaced : clusterNextToBePlaced)
		{
			if (geo_util::poly_util::polygonPolygonIntersectionArea(polygonNextToBePlaced, poly) > 0)
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

/** namespace polygon_fit */

Polygon polygon_fit::getNoFitPolygon(Polygon &referencePolygon, MultiPolygon &cluster)
{
	Polygon nfp = polygon_fit::getNoFitPolygon(referencePolygon, cluster[0]);
	;
	if (cluster.size() == 2)
	{
		nfp = polygon_fit::getNoFitPolygon(nfp, cluster[1]);
	}
	return nfp;
}

Polygon polygon_fit::getNoFitPolygon(Polygon &referencePolygon, Polygon &polygonToPlace)
{
	referencePolygon = geo_util::poly_util::translate(referencePolygon, Point(10000, 10000));
	polygonToPlace = geo_util::poly_util::translate(polygonToPlace, Point(10000, 10000));
	polygon_t polygonT_i = poly_t_util::convertToPolygon_t(referencePolygon);
	polygon_t polygonT_j = poly_t_util::convertToPolygon_t(polygonToPlace);

	nfp_t nfp = generateNFP(polygonT_i, polygonT_j);
	polygon_t nfpPolygonT = nfpRingsToNfpPoly(nfp);
	Polygon nfpPolygon = poly_t_util::convertToPolygon(nfpPolygonT);
	nfpPolygon = geo_util::poly_util::translate(nfpPolygon, Point(-10000, -10000));
	geo_util::poly_util::polygonRound(nfpPolygon);
	boost_geo::correct(nfpPolygon);
	return nfpPolygon;
}

/** namespace polygon_util */
vector<tuple<int, int, int, int>> cluster_util::getPerfectClustering(vector<vector<vector<vector<long double>>>> &clusterValues)
{
	int numberOfPolygons = clusterValues.size();
	int numberOfPairs = std::min(10, numberOfPolygons / 2);
	std::cout << numberOfPairs << "\n";
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
	for (auto polygon : allNfpIfr)
	{
		for (auto point : polygon.outer())
		{
			double _x = point.get<0>();
			double _y = point.get<1>();
			double l = geo_util::poly_util::getLength(cluster);
			double w = geo_util::poly_util::getWidth(cluster);
			if( _x < 0 or _y < 0 or _x + w > width or _y + l > length ) continue;
			candidatePlacementPositions.push_back(point);
		}
		for(auto inner: polygon.inners())
		{
			for(auto point: inner)
			{
				double _x = point.get<0>();
				double _y = point.get<1>();
				double l = geo_util::poly_util::getLength(cluster);
				double w = geo_util::poly_util::getWidth(cluster);
				if( _x < 0 or _y < 0 or _x + w > width or _y + l > length ) continue;
				candidatePlacementPositions.push_back(point);
			}
		}
	}
	for (auto point : allEdgeIntersectionPoints)
	{
		double _x = point.get<0>();
		double _y = point.get<1>();
		double l = geo_util::poly_util::getLength(cluster);
		double w = geo_util::poly_util::getWidth(cluster);
		if( _x < 0 or _y < 0 or _x + w > width or _y + l > length ) continue;
		candidatePlacementPositions.push_back(point);
	}
	return candidatePlacementPositions;
}
Point cluster_util::findBLFPoint(MultiPolygon &packing, MultiPolygon &cluster, long double length, long double width)
{
	Point blfPoint(INF, INF);
	vector<Point> candidatePlacementPositions = cluster_util::getCandidatePlacementPositions(packing, cluster, length, width);
	for (auto point : candidatePlacementPositions)
	{
		if (geo_util::poly_util::isItPossibleToPlacePolygon(packing, cluster, point) == true )
		{
			if (point.get<1>() < blfPoint.get<1>())
			{
				blfPoint = point;
			}
			else if ((point.get<1>() == blfPoint.get<1>()) and (point.get<0>() < blfPoint.get<0>()))
			{
				blfPoint = point;
			}
		}
	}
	return blfPoint;
}
MultiPolygon cluster_util::bottomLeftFill(vector<MultiPolygon> &clusters, long double length, long double width)
{
	MultiPolygon packing;
	for (auto cluster : clusters)
	{
		Point blfPoint = cluster_util::findBLFPoint(packing, cluster, length, width);
		// cluster = geo_util::poly_util::translate(cluster, blfPoint);
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
				continue;
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

					Polygon nfp = polygon_fit::getNoFitPolygon(polygon_i, polygon_j);

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
	vector<vector<vector<vector<long double>>>> clusterValues = cluster_util::getClusterValues(inputPolygons);
	vector<tuple<int, int, int, int>> clusterPairs = cluster_util::getPerfectClustering(clusterValues);

	vector<bool> taken((int)inputPolygons.size(), false);
	vector<MultiPolygon> clusters;
	int n = inputPolygons.size();

	for (auto &clusterId : clusterPairs)
	{
		int i, j, k, l;
		std::tie(i, j, k, l) = clusterId;
		MultiPolygon cluster;

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

		Polygon nfp = polygon_fit::getNoFitPolygon(polygon_i, polygon_j); //

		if (boost_geo::is_convex(nfp.outer()) == false)
		{
			taken[i] = taken[j] = true;
			Point dominantPoint = cluster_util::findDominantPoint(nfp);
			polygon_j = geo_util::poly_util::translate(polygon_j, dominantPoint);
			cluster.push_back(polygon_i);
			cluster.push_back(polygon_j);
			clusters.push_back(cluster);
		}
	}

	for (int i = 0; i < inputPolygons.size(); i++)
	{
		if (taken[i] == false)
		{
			clusters.push_back({inputPolygons[i]});
		}
	}
	cluster_util::sortByClusterValue(clusters);
	long double length = INITIAL_STOCK_LENGTH;
	MultiPolygon initialSolution; // = cluster_util::bottomLeftFill(clusters, length, width);
	return initialSolution;
}

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

	// polygon_fit::generateAllPairNfpForInputPolygons(polygons, datasetName, outputLocation);

	MultiPolygon initialPacking = cluster_util::generateInitialSolution(polygons, width);
	std::cout << boost_geo::wkt(initialPacking) << "\n";
}