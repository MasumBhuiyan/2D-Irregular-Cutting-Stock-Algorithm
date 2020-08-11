#include <bin_packing.hpp>
#include <libnfporb.hpp>

using namespace libnfporb;

double dblround(double x)
{
	double intPart = (long long)x;
	double frcPart = x - intPart;
	if (std::fabs(frcPart) < EPS)
	{
		return intPart;
	}
	return x;
}

/** namespace geo_util **/
int geo_util::dblcmp(double d, double eps)
{
	if (std::fabs(d) < eps)
	{
		return 0;
	}
	return d > eps ? 1 : -1;
}

bool geo_util::isAConcavePolygon(Polygon &polygon)
{
	return !(boost_geo::is_convex(polygon.outer()));
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
	double min_x = INF, min_y = INF;
	for (auto &point : polygon.outer())
	{
		min_x = std::min(min_x, point.get<0>());
		min_y = std::min(min_y, point.get<1>());
	}
	for (auto &innerPolygon : polygon.inners())
	{
		for(auto &point: innerPolygon)
		{
			min_x = std::min(min_x, point.get<0>());
			min_y = std::min(min_y, point.get<1>());
		}
	}
	for (auto &point : polygon.outer())
	{
		point = Point(point.get<0>() - min_x, point.get<1>() - min_y);
	}
	for (auto &innerPolygon : polygon.inners())
	{
		for(auto &point: innerPolygon)
		{
			point = Point(point.get<0>() - min_x, point.get<1>() - min_y);
		}
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
	return geo_util::dblcmp(p - EPS) < 0 ? -1 : 1;
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

Polygon geo_util::translatePolygon(Polygon polygon, Point translationPoint)
{
    Point reference = polygon.outer()[0];
	for (auto &point : polygon.outer())
	{
		Point t(point.get<0>() - reference.get<0>() + translationPoint.get<0>(), point.get<1>() - reference.get<1>() + translationPoint.get<1>());
		point = t;
	}
	return polygon;
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
		double a1 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(a.get<0>() - c.get<0>(), a.get<1>() - c.get<1>()));
		double a2 = geo_util::crossProduct(Point(d.get<0>() - c.get<0>(), d.get<1>() - c.get<1>()), Point(b.get<0>() - d.get<0>(), b.get<1>() - d.get<1>()));
		return Point((a.get<0>() * a2 - b.get<0>() * a1) / (a2 - a1), (a.get<1>() * a2 - b.get<1>() * a1) / (a2 - a1));
	}
	return Point(INF, INF);
}

vector<Polygon> geo_util::rotatePolygons(vector<Polygon> &polygons, Point reference, double degree)
{
	vector<Polygon> rotatedPolygons;
	for (auto &polygon : polygons)
	{
		rotatedPolygons.push_back(geo_util::rotatePolygon(polygon, reference, degree));
	}
	return rotatedPolygons;
}

MultiPolygon geo_util::translatePolygons(MultiPolygon cluster, Point translationPoint)
{
	Point reference = cluster[0].outer()[0];
	for (auto &polygon : cluster)
	{
		for (auto &point : polygon.outer())
		{
			Point t(point.get<0>() - reference.get<0>() + translationPoint.get<0>(), point.get<1>() - reference.get<1>() + translationPoint.get<1>());
			point = t;
		}
	}
	return cluster;
}

bool geo_util::isItPossibleToPlacePolygon(MultiPolygon &packing, MultiPolygon clusterNextToBePlaced, Point point)
{
	clusterNextToBePlaced = geo_util::translatePolygons(clusterNextToBePlaced, point);
	for (auto poly : packing)
	{
		for (auto polygonNextToBePlaced : clusterNextToBePlaced)
		{
			if ( geo_util::polygonPolygonIntersectionArea(polygonNextToBePlaced, poly) > 0)
			{
				return false;
			}
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
		//	assert(point.get<1>() >= 0);
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

void geo_util::visualize(MultiPolygon multipolygon, std::string location, std::string datasetName)
{
	Box box;
	boost::geometry::envelope(multipolygon, box);
	// << "make_envelope..............: " << boost::geometry::dsv(box) << std::endl;
	std::ostringstream name;
	name << "frame_" << std::setw(4) << std::setfill('0') << frameno++ << "_" << datasetName << ".svg";
	std::ofstream svg(location + name.str());
	boost_geo::svg_mapper<Point> mapper(svg, 700, 600);
	mapper.add(multipolygon);
	mapper.map(multipolygon, "fill-opacity:0.5;fill:rgb(153, 204,0);stroke:rgb(153,204,0);stroke-width:1", 5);
	mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(212,0,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
}

/** namespace polygon_fit **/
namespace nfp_util
{
	MultiPolygon convertNfp_t2MultiPolygon(nfp_t nfp)
	{
		MultiPolygon multiPolygon;
		for (auto &ring : nfp)
		{
			Polygon polygon;
			for (auto point : ring)
			{
				Point t((double)point.x_.val(), (double)point.y_.val());
				double _x = (double)t.get<0>();
				double _y = (double)t.get<1>();
				polygon.outer().push_back(Point(_x, _y));
			}
			if (polygon.outer().size())
			{
				polygon.outer().push_back(polygon.outer()[0]);
			}
			multiPolygon.push_back(polygon);
		}
		return multiPolygon;
	}

	polygon_t convertPolygon2Polygon_t(Polygon polygon)
	{
		polygon_t polygonT;
		for (auto &point : polygon.outer())
		{
			double _x = point.get<0>();
			double _y = point.get<1>();
			polygonT.outer().push_back(point_t(_x, _y));
		}
		return polygonT;
	}
	polygon_t convertNfp_t2Polygon_t(nfp_t nfp)
	{
		polygon_t nfppoly;
		for (const auto& pt : nfp.front()) {
			nfppoly.outer().push_back(pt);
		}
		for (size_t i = 1; i < nfp.size(); ++i) {
			nfppoly.inners().push_back({});
			for (const auto& pt : nfp[i]) {
				nfppoly.inners().back().push_back(pt);
			}
		}
		return nfppoly;
	}
}; // namespace nfp_util

/**
 * 
 */
Polygon polygon_fit::getInnerFitRectangle(MultiPolygon cluster, double length, double width)
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

vector<Point> polygon_fit::getAllEdgeIntersectionPoints(MultiPolygon &allNfpIfr)
{
	vector<tuple<Point, Point>> edges;
	vector<Point> allEdgeIntersectionPoints;

	for (auto &polygon : allNfpIfr)
	{
		int n = polygon.outer().size();
		for (int i = 0; i < n; i += 1)
		{
			edges.push_back({polygon.outer()[i], polygon.outer()[(i + 1) % n]});
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
			if ((inetersectionPoint.get<0>()) == INF and (inetersectionPoint.get<1>() == INF))
			{
				continue;
			}
			allEdgeIntersectionPoints.push_back(inetersectionPoint);
		}
	}
	return allEdgeIntersectionPoints;
}

/**
 * 
 */
 void correction(Polygon &polygon)
 {
	 for(auto &point: polygon.outer())
	 {
		 double _x = point.get<0>();
		 double _y = point.get<1>();
		 long long __x = _x;
		 long long __y = _y;
		 if( (_x - __x * 1.0) < EPS ) _x = __x;
		 if( (_y - __y * 1.0) < EPS ) _y = __y;
		 point = Point(_x, _y);
	 }
 }
Polygon polygon_fit::getNoFitPolygon(Polygon referencePolygon, MultiPolygon cluster)
{
	correction(referencePolygon);
	correction(cluster[0]);
	boost_geo::correct(referencePolygon);
	boost_geo::correct(cluster[0]);
	Polygon noFitPolygon;
	polygon_t polygon1, polygon2, polygon3;
	polygon1 = nfp_util::convertPolygon2Polygon_t(referencePolygon);
	polygon2 = nfp_util::convertPolygon2Polygon_t(cluster[0]);
	reverse(polygon1.outer().begin(), polygon1.outer().end());
	reverse(polygon2.outer().begin(), polygon2.outer().end());

	std::cout << "nfp polygon1: " << boost_geo::wkt(polygon1) << "\n";
	std::cout << "nfp polygon2: " << boost_geo::wkt(polygon2) << "\n";
	nfp_t nfp = generateNFP(polygon1, polygon2, true);
	if( cluster.size() == 2 )
	{
		boost_geo::correct(cluster[1]);
		correction(cluster[1]);
		polygon3 = nfp_util::convertPolygon2Polygon_t(cluster[1]);
		reverse(polygon3.outer().begin(), polygon3.outer().end());
		polygon_t polygon4 = nfp_util::convertNfp_t2Polygon_t(nfp);
		nfp = generateNFP(polygon4, polygon3);
	}

	for(const auto& point: nfp.front())
	{
		Point t((double)point.x_.val(), (double)point.y_.val());
		noFitPolygon.outer().push_back(t);
	}
	reverse(noFitPolygon.outer().begin(), noFitPolygon.outer().end());
	for (size_t i = 1; i < nfp.size(); i += 1) 
	{
		noFitPolygon.inners().push_back({});
		for (const auto& point: nfp[i]) 
		{
			Point t((double)point.x_.val(), (double)point.y_.val());
			noFitPolygon.inners().back().push_back(t);
		}
		reverse(noFitPolygon.inners().back().begin(), noFitPolygon.inners().back().end());
	}
	return noFitPolygon;
}

/**
 * 
 */
MultiPolygon polygon_fit::getAllNfpIfr(MultiPolygon &packing, MultiPolygon cluster, double length, double width)
{
	MultiPolygon allNfpIfr;
	Polygon ifr = polygon_fit::getInnerFitRectangle(cluster, length, width);
	allNfpIfr.push_back(ifr);
	std::cout << "\ngetAllNfpIfr: " << boost_geo::wkt(ifr) << " length = " << length << " width = " << width << "\n";
	for (auto &polygon : packing)
	{
		Polygon nfp = polygon_fit::getNoFitPolygon(polygon, cluster);
		std::cout << "getAllNfpIfr: " << boost_geo::wkt(nfp) << " length = " << length << " width = " << width << "\n";
		allNfpIfr.push_back(nfp);
	}
	return allNfpIfr;
}

/** namespace cluster_util **/
/**
 * returns the convex hull of the multipolygon
 */
Polygon cluster_util::convexHull(MultiPolygon multiPolygon)
{
	Polygon convexhull;
	boost_geo::convex_hull(multiPolygon, convexhull);
	return convexhull;
}

/**
 * retruns convex hull vacaencies
 */
MultiPolygon cluster_util::findConvexHullVacancy(Polygon &concavePolygon)
{
	MultiPolygon vacancies;
	if( geo_util::isAConcavePolygon(concavePolygon) == false ) return vacancies;
	Polygon convexhull = cluster_util::convexHull({concavePolygon});
	boost_geo::difference(convexhull, concavePolygon, vacancies);
	return vacancies;
}

/**
 * 
 */
vector<tuple<Point, Point>> cluster_util::findOppositeSideOfVacancies(Polygon &concavePolygonConvexHull, MultiPolygon &convexHullVacancies)
{
	vector<tuple<Point, Point>> oppositeSideOfVacancies;
	if( convexHullVacancies.size() == 0 ) 
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
			double distance = geo_util::linePointDistance(p, q, point);
			if (distance > maxDistance)
			{
				maxDistance = distance;
				dominant = point;
			}
		}
	}
	return dominant;
}

/**
 * 
 */
double cluster_util::getClusteringCriteria1(Polygon &polygon1, Polygon &polygon2)
{
	double value = 0.0, value1 = 0.0, value2 = 0.0;
	double areaIntersection1 = 0.0, totalVacancy1 = 0.0;
	double areaIntersection2 = 0.0, totalVacancy2 = 0.0;

	vector<Polygon> V1 = cluster_util::findConvexHullVacancy(polygon1);
	vector<Polygon> V2 = cluster_util::findConvexHullVacancy(polygon2);

	for (auto &vacancy : V2)
	{
		double area = geo_util::polygonPolygonIntersectionArea(polygon1, vacancy);
		if (area > 0)
		{
			areaIntersection1 += area;
			totalVacancy1 += std::fabs(boost_geo::area(vacancy));
		}
	}
	for (auto &vacancy : V1)
	{
		double area = geo_util::polygonPolygonIntersectionArea(polygon2, vacancy);
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

/**
 * 
 */
double cluster_util::getClusteringCriteria2(Polygon &polygon1, Polygon &polygon2)
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
/**
 * 
 */
double cluster_util::getClusterValue(Polygon &polygon1, Polygon &polygon2)
{
	double value = cluster_util::getClusteringCriteria1(polygon1, polygon2) + cluster_util::getClusteringCriteria2(polygon1, polygon2);
	return value;
}

/**
 * 
 */
void cluster_util::sortByClusterValue(vector<MultiPolygon> &clusters)
{
	sort(clusters.begin(), clusters.end(), [](MultiPolygon cluster1, MultiPolygon cluster2) {
		double area1 = 0.0, area2 = 0.0;
		for (auto polygon : cluster1)
		{
			correction(polygon);
			area1 += std::fabs(boost_geo::area(polygon));
		}
		for (auto polygon : cluster2)
		{
			correction(polygon);
			area2 += std::fabs(boost_geo::area(polygon));
		}
		// area1 = std::fabs(boost_geo::area(cluster1));
		// area2 = std::fabs(boost_geo::area(cluster2));
		return geo_util::dblcmp(area1 - area2, EPS) > 0;
	});
}
/**
 * 
 */
vector<Point> cluster_util::getCandidatePlacementPositions(MultiPolygon &packing, MultiPolygon &cluster, double length, double width)
{
	vector<Point> candidatePlacementPositions;
	MultiPolygon allNfpIfr = polygon_fit::getAllNfpIfr(packing, cluster, length, width);
	vector<Point> allEdgeIntersectionPoints = polygon_fit::getAllEdgeIntersectionPoints(allNfpIfr);
	for (auto polygon : allNfpIfr)
	{
		// std::cout << "allNfpIfr: " << boost_geo::wkt(polygon) << "\n";
		for (auto point : polygon.outer())
		{
			// if( boost_geo::within(point, allNfpIfr[0]) == false) continue;
			candidatePlacementPositions.push_back(point);
		}
	}
	for (auto point : allEdgeIntersectionPoints)
	{
		// std::cout << "intersection points: " << boost_geo::wkt(point) << " " << boost_geo::within(point, allNfpIfr[0]) << "\n";
		// if( boost_geo::within(point, allNfpIfr[0]) == false) continue;
		candidatePlacementPositions.push_back(point);
	}
	return candidatePlacementPositions;
}
/**
 * 
 */
Point cluster_util::findBLFPoint(MultiPolygon &packing, MultiPolygon &cluster, double length, double width)
{
	Point blfPoint(INF, INF);
	vector<Point> candidatePlacementPositions = cluster_util::getCandidatePlacementPositions(packing, cluster, length, width);
	for (auto point : candidatePlacementPositions)
	{
		if( point.get<0>() < 0 or point.get<0>() > width ) continue;
		if( point.get<1>() < 0 or point.get<1>() > length ) continue;
		if (geo_util::isItPossibleToPlacePolygon(packing, cluster, point) == true )
		{
			std::cout << "    candidate point: " << boost_geo::wkt(point) << "\n";
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
/**
 * 
 */
MultiPolygon cluster_util::bottomLeftFill(vector<MultiPolygon> &clusters, double length, double width)
{
	MultiPolygon packing;
	for (auto cluster : clusters)
	{
		Point blfPoint = cluster_util::findBLFPoint(packing, cluster, length, width);

		std::cout << "blf point: " << boost_geo::wkt(blfPoint) << "\n";
		MultiPolygon translatedCluster = geo_util::translatePolygons(cluster, blfPoint);
		for (auto polygon : translatedCluster)
		{
			packing.push_back(polygon);
		}
	}
	return packing;
}
/**
 * 
 */
double cluster_util::findBestPairWiseClusters(vector<vector<vector<vector<double>>>> &clusterValues, vector<double> &dp, int numberOfPairs, int mask)
{
	int taken = __builtin_popcount(mask);
	if (taken == (numberOfPairs + numberOfPairs))
	{
		return 0.0;
	}
	double &maxProfit = dp[mask];
	if (maxProfit != -1.0)
	{
		return maxProfit;
	}
	maxProfit = -INF;

	int POLYGON_SIZE = std::min(20,(int)clusterValues.size());
	 // std::cout << "POLYGON SIZE: " << POLYGON_SIZE << "\n";
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
					double __maxProfit = cluster_util::findBestPairWiseClusters(clusterValues, dp, numberOfPairs, mask ^ bitOn12) + clusterValues[i][j][k][l];
					maxProfit = std::max(maxProfit, __maxProfit);
				}
			}
		}
	}
	return maxProfit;
}
/**
 * 
 */
void cluster_util::printBestPairWiseClusters(vector<vector<vector<vector<double>>>> &clusterValues, vector<double> &dp, int numberOfPairs, int mask, vector<tuple<int, int, int, int>> &clusterPairs)
{
	int taken = __builtin_popcount(mask);
	if (taken == (numberOfPairs + numberOfPairs))
	{
		return;
	}
	double &maxProfit = dp[mask];
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
					double __maxProfit = cluster_util::findBestPairWiseClusters(clusterValues, dp, numberOfPairs, mask ^ bitOn12) + clusterValues[i][j][k][l];
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
/**
 * 
 */
vector<tuple<int, int, int, int>> cluster_util::getPerfectClustering(vector<vector<vector<vector<double>>>> &clusterValues, double width)
{
	int numberOfPolygons = clusterValues.size();
	int numberOfPairs = std::min(10, numberOfPolygons / 2);
	std::cout << numberOfPairs << "\n";
	vector<tuple<int, int, int, int>> clusterPairs;
	vector<double> dp(1 << 22, -1.0);
	std::cout << "findingBestPairs\n";
	double bestClusterizationValue = cluster_util::findBestPairWiseClusters(clusterValues, dp, numberOfPairs, 0);
	std::cout << "printingBestPairs: " << bestClusterizationValue << "\n";
	cluster_util::printBestPairWiseClusters(clusterValues, dp, numberOfPairs, 0, clusterPairs);
	return clusterPairs;
}
/**
 * 
 */
vector<vector<vector<vector<double>>>> cluster_util::getClusterValues(vector<Polygon> &inputPolygons, std::string datasetName)
{
	int n = inputPolygons.size();
	vector<vector<vector<vector<double>>>> clusterValues;
	clusterValues.resize(n, vector<vector<vector<double>>>(n, vector<vector<double>>(ALLOWABLE_ROTATIONS.size(), vector<double>(ALLOWABLE_ROTATIONS.size(), 0.0))));
	for (int i = 0; i < n; i += 1)
	{
		Polygon polygon_i = inputPolygons[i];
		for (int j = 0; j < n; j += 1)
		{
			Polygon polygon_j = inputPolygons[j];
			if (i != j)
			{
				for (int k = 0; k < ALLOWABLE_ROTATIONS.size(); k += 1)
				{
					for (int l = 0; l < ALLOWABLE_ROTATIONS.size(); l += 1)
					{
						int nfp_file_no =(l + k * ALLOWABLE_ROTATIONS.size() + j * ALLOWABLE_ROTATIONS.size() * ALLOWABLE_ROTATIONS.size()  + i * ALLOWABLE_ROTATIONS.size() * ALLOWABLE_ROTATIONS.size() * n);
						polygon_t nfp;
						std::string filename = "../io/" + datasetName + "/" + std::to_string(nfp_file_no) + ".wkt";
						read_wkt_polygon(filename, nfp);
						
						Polygon nfpPolygon;
						for(auto point: nfp.outer()) nfpPolygon.outer().push_back(Point((double)point.x_.val(), (double)point.y_.val()));
						for(auto inner: nfp.inners())
						{
							nfpPolygon.inners().push_back({});
							for(auto point: inner)
							{
								nfpPolygon.inners().back().push_back(Point((double)point.x_.val(), (double)point.y_.val()));
							}
						}
						boost_geo::correct(nfpPolygon);
						if ( geo_util::isAConcavePolygon(nfpPolygon) == true )
						{
							Point point = cluster_util::findDominantPoint(nfpPolygon);
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
///////////////////////////////////////////////////////////////////////////// EXTRA /////////////////////////////////////////////////
void translate(polygon_t &polygon, point_t tp)
{
	double __x = (double)polygon.outer()[0].x_.val();
	double __y = (double)polygon.outer()[0].y_.val();
	
	for(auto& point: polygon.outer())
	{
		double _x =(double) point.x_.val();
		double _y =(double) point.y_.val();

		_x = _x - __x + (double)tp.x_.val();
		_y = _y - __y + (double)tp.y_.val();
		dblround(_x);
		dblround(_y);
		point = point_t(_x, _y);
	}
	for(auto& inner: polygon.inners())
	{
		for(auto& point: inner)
		{
			double _x =(double) point.x_.val();
			double _y =(double) point.y_.val();
			_x = _x - __x + (double)tp.x_.val();
			_y = _y - __y + (double)tp.y_.val();
			dblround(_x);
			dblround(_y);
			point = point_t(_x, _y);
		}
	}
}
void rotate(double &x, double &y, double angle){
	// std::cout << x << " " << y << " : ";
	angle = (angle / 180.0) * acos(-1.0);
	double c = cos(angle), s = sin(angle);
	double _x = x * c - y * s;
	double _y = x * s + y * c;
	x = _x;
	y = _y;

	x = dblround(x);
	y = dblround(y);
}
void rotateCW(polygon_t &polygon, double rotationAngleInDegree)
{
	for(auto& point: polygon.outer())
	{
		double _x =(double) point.x_.val();
		double _y =(double) point.y_.val();

		rotate(_x, _y, rotationAngleInDegree);
		point = point_t(_x, _y);
	}
	for(auto& inner: polygon.inners())
	{
		for(auto& point: inner)
		{
			double _x =(double) point.x_.val();
			double _y =(double) point.y_.val();
			rotate(_x, _y, rotationAngleInDegree);
			point = point_t(_x, _y);
		}
	}
}
void vis1(Polygon polygon, MultiPolygon cluster, Polygon nfp, std::string location, int testId)
{
    Box box;
    MultiPolygon multipolygon;
    // for(auto point: nfp.outer())
    // {
    //     MultiPolygon polygons = geo_util::translatePolygons(cluster, point);     
    //     for(auto poly: polygons)
    //     {
    //         multipolygon.push_back(poly);
    //     }   
    // }
    // for(auto innerPoly: nfp.inners())
    // {
    //     for(auto point: innerPoly)
    //     {
    //         MultiPolygon polygons = geo_util::translatePolygons(cluster, point);     
    //         for(auto poly: polygons)
    //         {
    //             multipolygon.push_back(poly);
    //         }   
    //     }
    // }
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * given a set of polygons and width
 * retruns an initial bin packing
 */
MultiPolygon cluster_util::generateInitialSolution(vector<Polygon> &inputPolygons, double width)
{
	std::cout << "generate initial solution running..." << std::endl;
	vector<vector<vector<vector<double>>>> clusterValues = cluster_util::getClusterValues(inputPolygons, "jakobs1");

	std::cout << "cluster pairs receiving..." << std::endl;
	vector<tuple<int, int, int, int>> clusterPairs = cluster_util::getPerfectClustering(clusterValues, std::min((int)inputPolygons.size(), 10));
	
	vector<bool> taken((int)inputPolygons.size(), false);
	vector<MultiPolygon> clusters;
	int n = inputPolygons.size();
	int ID = 0;
	for (auto &clusterId : clusterPairs)
	{
		int i, j, k, l;
		std::tie(i, j, k, l) = clusterId;
		MultiPolygon cluster;

		Polygon polygon_i = geo_util::rotatePolygon(inputPolygons[i], Point(0,0), -ALLOWABLE_ROTATIONS[k]);
		Polygon polygon_j = geo_util::rotatePolygon(inputPolygons[j], Point(0,0), -ALLOWABLE_ROTATIONS[l]);
		
		//////////////////////////////////////////////
		int nfp_file_no =(l + k * ALLOWABLE_ROTATIONS.size() + j * ALLOWABLE_ROTATIONS.size() * ALLOWABLE_ROTATIONS.size()  + i * ALLOWABLE_ROTATIONS.size() * ALLOWABLE_ROTATIONS.size() * n);
		polygon_t nfp;
		std::string filename = "../io/jakobs1/" + std::to_string(nfp_file_no) + ".wkt";
		read_wkt_polygon(filename, nfp);
						
		Polygon nfpPolygon;
		for(auto point: nfp.outer()) 
		{
			double _x = dblround(point.x_.val());
			double _y = dblround(point.y_.val());
			nfpPolygon.outer().push_back(Point(_x, _y));
		}
		for(auto inner: nfp.inners())
		{
			nfpPolygon.inners().push_back({});
			for(auto point: inner)
			{
				double _x = dblround(point.x_.val());
				double _y = dblround(point.y_.val());
				nfpPolygon.inners().back().push_back(Point(_x, _y));
			}
		}
		boost_geo::correct(nfpPolygon);


		/////////////////////////////////////////
		if (geo_util::isAConcavePolygon(nfpPolygon) == true)
		{
			polygon_i = geo_util::translatePolygon(polygon_i, Point(0,0));
			std::cout << " NFP: " <<  boost_geo::wkt(nfpPolygon) << "\n";
			std::cout << " POL: " << boost_geo::wkt(polygon_i) << "\n";
			taken[i] = taken[j] = true;
			Point point = cluster_util::findDominantPoint(nfpPolygon);
			polygon_j = geo_util::translatePolygon(polygon_j, point);
			vis1(polygon_i, {polygon_j}, nfpPolygon, "../tests/results/", ID++);
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

	std::cout << "sorting....\n";
	std::cout << "<------------------CLUSTERS--------------->\n";
	std::cout << clusters.size() << "\n";
	for(auto p: clusters)
	{
		for(auto _p: p) correction(_p);
		std::cout << boost_geo::wkt(p) << "\n";
	}
	std::cout << "ends\n";
	cluster_util::sortByClusterValue(clusters);
	std::cout << "sorted.....\n";

	std::cout << "<------------------CLUSTERS--------------->\n";
	for(auto p: clusters)
	{
		for(auto _p: p) correction(_p);
		std::cout << boost_geo::wkt(p) << "\n";

		//geo_util::visualize(p, "../tests/results/", "jakobs1");
	}
	double length = INITIAL_STOCK_LENGTH;
	MultiPolygon initialSolution;// = cluster_util::bottomLeftFill(clusters, length, width);
	for(auto poly: initialSolution)
	{
		std::cout << boost_geo::wkt(poly) << "\n";
	}
	// geo_util::visualize(initialSolution, "../tests/results/", "initialSol");
	return initialSolution;
}

// /** namespace bin_packing */
// /** 
//  * read input from dataset file
//  * file location ../io/datasets/**.txt
//  */
std::tuple<vector<Polygon>, double> bin_packing::readDataset(std::string datasetName)
{
	double width;
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
				double _x, _y;
				file >> _x >> _y;
				polygon.outer().push_back(Point(_x, _y));
			}
			polygon.outer().push_back(polygon.outer()[0]);
			boost_geo::correct(polygon);
			geo_util::translatePolygon(polygon, Point(0,0));
			while (quantity--)
			{
				polygons.push_back(polygon);
			}
		}
	}
	file.close();
	return {polygons, width};
}
// /**
//  * checks feasibilty of a packing
//  * compares with FEASIBILTY limit
//  */
// bool bin_packing::isFeasible(MultiPolygon &packing, double totalAreaOfInputPolygons)
// {
// 	double overlappingArea = 0.0;
// 	double feasibilityRatio = 0.0;
// 	int n = packing.size();
// 	for (int i = 0; i < n; i += 1)
// 	{
// 		for (int j = i + 1; j < n; j += 1)
// 		{
// 			overlappingArea += geo_util::polygonPolygonIntersectionArea(packing[i], packing[j]);
// 		}
// 	}
// 	feasibilityRatio = overlappingArea / totalAreaOfInputPolygons;
// 	return geo_util::dblcmp(feasibilityRatio - FEASIBILTY, EPS) >= 0 ? true : false;
// }
// /**
//  * naive implementation
//  * may be changed in future
//  */
// double bin_packing::getPenetrationDepth(Polygon polygonA, Polygon polygonB)
// {
// 	MultiPolygon intersections;
// 	boost_geo::intersection(polygonA, polygonB, intersections);
// 	return std::fabs(boost_geo::area(intersections));
// }
// /**
//  * returns total penitration depth of a pcking
//  */
// double bin_packing::getTotalPenetrationDepth(MultiPolygon &packing)
// {
// 	double totalPenetrationDepth = 0.0;
// 	int n = packing.size();
// 	for (int i = 0; i < n; i += 1)
// 	{
// 		for (int j = i + 1; j < n; j += 1)
// 		{
// 			totalPenetrationDepth += bin_packing::getPenetrationDepth(packing[i], packing[j]);
// 		}
// 	}
// 	return totalPenetrationDepth;
// }
// /**
//  * 
//  */
// double bin_packing::getOverlapPenalty(MultiPolygon &packing, vector<vector<double>> &penalty, int id, double rotationAngle, Point translationPoint)
// {
// 	int n = packing.size();
// 	Polygon polygon = packing[id];
// 	polygon = geo_util::rotatePolygon(polygon, polygon.outer()[0], rotationAngle);
// 	polygon = geo_util::translatePolygon(polygon, translationPoint);

// 	double overlapPenalty = 0;
// 	for (int j = 0; j < n; j++)
// 	{
// 		overlapPenalty += (penalty[id][j] * bin_packing::getPenetrationDepth(polygon, packing[j]));
// 	}
// 	return overlapPenalty;
// }
// void bin_packing::increasePenalty(MultiPolygon &packing, vector<vector<double>> &penalty)
// {
// 	int n = penalty.size();
// 	double maximumPenetrationDepth = -INF;
// 	vector<vector<double>> penetrationDepths(n, vector<double>(n, 0));
// 	for (int i = 0; i < n; i++)
// 	{
// 		for (int j = 0; j < n; j++)
// 		{
// 			penetrationDepths[i][j] = bin_packing::getPenetrationDepth(packing[i], packing[j]);
// 			if (i < j)
// 			{
// 				maximumPenetrationDepth = std::max(maximumPenetrationDepth, penetrationDepths[i][j]);
// 			}
// 		}
// 	}
// 	for (int i = 0; i < n; i++)
// 	{
// 		for (int j = 0; j < n; j++)
// 		{
// 			penalty[i][j] += (penetrationDepths[i][j] / maximumPenetrationDepth);
// 		}
// 	}
// }
// Point bin_packing::cuckooSearch(MultiPolygon &packing, vector<vector<double>> &penalty, int id, double rotationAngle, double width, double lenght)
// {
// 	Polygon polygon = packing[id];
// 	polygon = geo_util::rotatePolygon(polygon, polygon.outer()[0], rotationAngle);
// 	Polygon innerFitRectangle = polygon_fit::getInnerFitRectangle({polygon}, lenght, width);
// 	assert(innerFitRectangle.outer().size() == 5);
// 	double max_x = -INF, min_x = INF, max_y = -INF, min_y = INF;
// 	for (Point &point : polygon.outer())
// 	{
// 		max_x = std::max(max_x, point.get<0>());
// 		min_x = std::min(min_x, point.get<0>());
// 		max_y = std::max(max_y, point.get<1>());
// 		min_y = std::min(min_y, point.get<1>());
// 	}

// 	vector<Point> hostNests(NUMBER_OF_HOST_NESTS);
// 	for (int i = 0; i < NUMBER_OF_HOST_NESTS; i++)
// 	{
// 		srand(time(NULL));
// 		hostNests[i] = {std::rand() % (int)(max_x - min_x + 1) + min_x, std::rand() % (int)(max_y - min_y + 1) + min_y};
// 	}
// 	// Begin cuckoo search
// 	Point bestPosition = packing[id].outer()[0];
// 	for (int t = 0; t < MAXIMUM_GENERATION; t++)
// 	{
// 		srand(time(NULL));
// 		int i = std::rand() % NUMBER_OF_HOST_NESTS;
// 		double overlapPenalty_i = bin_packing::getOverlapPenalty(packing, penalty, id, rotationAngle, hostNests[i]);

// 		srand(time(NULL));
// 		int j = std::rand() % NUMBER_OF_HOST_NESTS;
// 		double overlapPenalty_j = bin_packing::getOverlapPenalty(packing, penalty, id, rotationAngle, hostNests[j]);

// 		if (overlapPenalty_i < overlapPenalty_j)
// 		{
// 			hostNests[j] = hostNests[i];
// 			bestPosition = hostNests[j];
// 		}
// 		// Abandon a fraction (Pa) of worse nests and build new ones
// 		// at new locations
// 		// Keep the best solutions (or nests with quality solutions)
// 		// Rank the solutions and find the current best
// 	}
// 	return bestPosition;
// }
// MultiPolygon bin_packing::minimizeOverlap(MultiPolygon packing, vector<double> allowableRoatations, double width, double length)
// {
// 	int n = packing.size();
// 	vector<vector<double>> penalty(n, vector<double>(n, 1.0));
// 	int it = 0;
// 	double fitness = INF;
// 	vector<int> Q(n);
// 	for (int i = 0; i < n; i++)
// 	{
// 		Q[i] = i;
// 	}
// 	while (it < MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA)
// 	{
// 		std::random_shuffle(Q.begin(), Q.end());
// 		for (int i = 0; i < n; i++)
// 		{
// 			double overlapPenalty = bin_packing::getOverlapPenalty(packing, penalty, Q[i], 0, packing[Q[i]].outer()[0]);
// 			Point bestLocation(INF, INF);
// 			double bestRotationAngle = 0.0;

// 			for (double rotationAngle : allowableRoatations)
// 			{
// 				Point translationPoint = bin_packing::cuckooSearch(packing, penalty, Q[i], rotationAngle, width, length);
// 				double currentOverlapPenalty = bin_packing::getOverlapPenalty(packing, penalty, Q[i], rotationAngle, translationPoint);
// 				if (currentOverlapPenalty < overlapPenalty)
// 				{
// 					overlapPenalty = currentOverlapPenalty;
// 					bestLocation = translationPoint;
// 					bestRotationAngle = rotationAngle;
// 				}
// 			}
// 			packing[Q[i]] = geo_util::rotatePolygon(packing[Q[i]], packing[Q[i]].outer()[0], bestRotationAngle);
// 			packing[Q[i]] = geo_util::translatePolygon(packing[Q[i]], bestLocation);
// 		}
// 		double totalPenetrationDepth = bin_packing::getTotalPenetrationDepth(packing);
// 		if (geo_util::dblcmp(totalPenetrationDepth) == 0)
// 		{
// 			return packing;
// 		}
// 		else if (totalPenetrationDepth < fitness)
// 		{
// 			fitness = totalPenetrationDepth;
// 			it = 0;
// 		}

// 		bin_packing::increasePenalty(packing, penalty); // increase penalty
// 		it += 1;
// 	}
// 	return packing;
// }
void bin_packing::binPacking(vector<Polygon> &polygons, double width, std::string location, std::string datasetName, double runTimeDuration)
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

	double bestLength = geo_util::getPackingLength(initialPacking);
	double decreasingRate = 0.1;
	double increasingRate = 0.1;
	double currentLength = (1.0 - decreasingRate) * bestLength;

	// while (true)
	// {
	// 	auto stop = std::chrono::high_resolution_clock::now();
	// 	auto duration = std::chrono::duration_cast<std::chrono::minutes>(stop - start);
	// 	if (duration.count() >= runTimeDuration)
	// 	{
	// 		break;
	// 	}
	// 	currentPacking = bin_packing::minimizeOverlap(currentPacking, ALLOWABLE_ROTATIONS, width, currentLength);
	// 	if (bin_packing::isFeasible(currentPacking, totalAreaOfInputPolygons))
	// 	{
	// 		bestPacking = currentPacking;
	// 		bestLength = currentLength;
	// 		currentLength = (1.0 - decreasingRate) * bestLength;
	// 	}
	// 	else
	// 	{
	// 		currentLength = (1.0 + increasingRate) * bestLength;
	// 	}
	// }
    double bestPackingArea = boost_geo::area(bestPacking);
    Box stock;
    boost_geo::envelope(bestPacking, stock);
    double stockArea = boost_geo::area(stock);
    Point stockDimension = stock.max_corner();
    boost_geo::subtract_point(stockDimension, stock.min_corner());

	std::cout << "\033[1;36mwidth......................: " << width << "\033[0m" << std::endl;
	std::cout << "\033[1;36mlength.....................: " << bestLength << "\033[0m" << std::endl;
    std::cout << "\033[1;36mtotal area of items........: " << totalAreaOfInputPolygons << "\033[0m" << std::endl;
    std::cout << "\033[1;36mresult polygon set area....: " << bestPackingArea << "\033[0m" << std::endl;
    std::cout << "\033[1;36mstock dimension [l * w]....: "
              << "[" << std::fabs(stockDimension.get<0>()) << ", " << std::fabs(stockDimension.get<1>()) << "]" << "\033[0m" << std::endl;
    std::cout << "\033[1;36mstock area.................: " << stockArea << "\033[0m" << std::endl;
    std::cout << "\033[1;32mpacking density............: " << (totalAreaOfInputPolygons / stockArea) * 100 << " %\033[0m" << std::endl;
	geo_util::visualize(bestPacking, location, datasetName);
}
