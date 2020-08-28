#include <bin_packing.hpp>

int geo_util::dblcmp(long double d, long double eps)
{
	// std::cout << std::setprecision(10) << std::fixed << " D = " << d << " " << (std::fabs(d) < EPS) << "\n";
	if (std::fabs(d) < EPS)
	{
		return 0;
	}
	return d > EPS ? 1 : -1;
}

long double geo_util::dblround(long double x, long double eps)
{
	long double intPart = (long long)x;
	long double frcPart = x - intPart;

	if (std::fabs(x) < eps)
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

Point geo_util::segmentSegmentIntersectionPoint(Point a, Point b, Point c, Point d)
{
	int o1 = geo_util::orientation(a, b, c), o2 = geo_util::orientation(a, b, d);
	int o3 = geo_util::orientation(c, d, a), o4 = geo_util::orientation(c, d, b);

	if ((!o1 && geo_util::pointInRectangle(a, b, c)))
	{
		return c;
	}
	if ((!o2 && geo_util::pointInRectangle(a, b, d)))
	{
		return d;
	}
	if ((!o3 && geo_util::pointInRectangle(c, d, a)))
	{
		return a;
	}
	if ((!o4 && geo_util::pointInRectangle(c, d, b)))
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


void geo_util::poly_util::readWKTMultiPolygon(MultiPolygon &multiPolygon, string filename)
{
	std::ifstream multiPolygonWKTFile(filename);
	assert(multiPolygonWKTFile.is_open());

	string wktStr;
	multiPolygonWKTFile.seekg(0, std::ios::end);
	wktStr.reserve(multiPolygonWKTFile.tellg());
	multiPolygonWKTFile.seekg(0, std::ios::beg);
	wktStr.assign((std::istreambuf_iterator<char>(multiPolygonWKTFile)), std::istreambuf_iterator<char>());
	wktStr.pop_back();
	boost_geo::read_wkt(wktStr, multiPolygon);
	boost_geo::correct(multiPolygon);

	multiPolygonWKTFile.close();
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
	boost_geo::multiply_value(referencePoint, -1);
	rotatedPolygon = geo_util::poly_util::translate(rotatedPolygon, referencePoint);
	return rotatedPolygon;
}

MultiPolygon geo_util::poly_util::rotateCW(MultiPolygon multiPolygon, long double rotationAngleInDegree, Point referencePoint)
{
	for (auto &polygon : multiPolygon)
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
	// std::cout << "make_envelope..............: " << boost::geometry::dsv(box) << std::endl;
	std::ostringstream name;
	name << "frame_" << std::setw(4) << std::setfill('0') << frameno++ << "_" << datasetName << ".svg";
	std::ofstream svg(outputLocation + "/" + name.str());
	boost_geo::svg_mapper<Point> mapper(svg, 400, 500);
	mapper.add(multiPolygon);
	mapper.map(multiPolygon, "fill-opacity:0.5;fill:rgb(169,169,169);stroke:rgb(169,169,169);stroke-width:1");
	mapper.map(box, "opacity:0.8;fill:none;stroke:rgb(0,0,0);stroke-width:1;stroke-linecap:round");
}

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
	std::cout << "Reading completed"
			  << "...\n";
	return {polygons, width};
}

long double bin_packing::getPackingDensity(MultiPolygon &packing)
{
	Box stock;
	boost_geo::envelope(packing, stock);
	long double stockArea = std::fabs(boost_geo::area(stock));
	long double polygonsArea = std::fabs(boost_geo::area(packing));
	long double packingDensity = polygonsArea / stockArea;
	return packingDensity * 100.0;
}

void bin_packing::increasePenalty(MultiPolygon &packing, vector<vector<long double>> &penalty, vector<vector<long double>>& depths)
{
	int n = penalty.size();
	long double maxDepth = -INF;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			maxDepth = std::max(maxDepth, depths[i][j]);
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			penalty[i][j] += (depths[i][j] / maxDepth);
		}
	}
}

void bin_packing::pushDown(MultiPolygon &packing, double length)
{
	for (auto &polygon : packing)
	{
		double pushDownBy = 0;
		for (auto &point : polygon.outer())
		{
			double dif = point.get<1>() - length;
			if (geo_util::dblcmp(dif, EPS) > 0)
			{
				pushDownBy = std::max(pushDownBy, dif);
			}
		}
		for (auto &point : polygon.outer())
		{
			point = Point(point.get<0>(), point.get<1>() - pushDownBy);
		}
		for (auto &inner : polygon.inners())
		{
			for (auto &point : inner)
			{
				point = Point(point.get<0>(), point.get<1>() - pushDownBy);
			}
		}
	}
}
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
long double randomInRange(long double a, long double b)
{
	long double random = ((long double)std::rand()) / (long double)RAND_MAX;
	long double diff = b - a;
	long double r = random * diff;
	return a + r;
}

long double getDepth(Polygon polygon1, Polygon polygon2)
{
	long double depth = geo_util::poly_util::polygonPolygonIntersectionArea(polygon1, polygon2);
	return depth;
}

Polygon rotateTranslate(Polygon polygon, Point v, long double o)
{
		Point reference = polygon.outer()[0];
		polygon = geo_util::poly_util::rotateCW(polygon, o, reference);
		boost_geo::multiply_value(reference, -1);
		polygon = geo_util::poly_util::translate(polygon, reference);
		polygon = geo_util::poly_util::translate(polygon, v);
		return polygon;
}

long double evaluate(MultiPolygon& packing, int polygonId, Point v, long double rotationAngleInDegree)
{
	Polygon polygon = rotateTranslate(packing[polygonId], v, rotationAngleInDegree);
	long double eval = 0.0;
	int n = packing.size();
	for(int i = 0; i < n; i += 1)
	{
		if( i != polygonId )
		{	
			eval += getDepth(packing[i], polygon);
		}
	}
	return eval;
}

long double evaluateAll(MultiPolygon& packing, vector<vector<long double>>& penalty, vector<vector<long double>>& depths)
{
	long double fitness = 0.0;
	int n = packing.size();
	for(int i = 0; i < n; i += 1)
	{
		for(int j = i + 1; j < n; j += 1)
		{	
			long double depth = getDepth(packing[i], packing[j]);
			depths[i][j] = depth;
			depths[j][i] = depth;
			fitness += depth * penalty[i][j];
		}
	}

	return fitness;
}

Point getCuckoo(long double x1, long double x2, long double y1, long double y2)
{
	long double x = randomInRange(x1, x2);
	long double y = randomInRange(y1, y2);
	if( x < x1 ) x = x1;
	else if( x > x2 ) x = x2;

	if(y < y1) y = y1;
	else if( y > y2 ) y = y2;

	return Point(x, y);
}

Point normalDistribution(long double& sigma, bool multiplicate)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0, 1.0);

	Point u;
	if( multiplicate == true )
	{
		u = Point(distribution(generator) * sigma, distribution(generator) * sigma);  
	}	
	else
	{
		u = Point(distribution(generator), distribution(generator));  
	}
	return u;
}

Point getInBoundary(Point a, Point b, long double x1, long double x2, long double y1, long double y2)
{
	Point intersection;

	intersection = geo_util::segmentSegmentIntersectionPoint(a, b, Point(x1, y1), Point(x1, y2));
	if(intersection.get<0>() != INF and intersection.get<1>() != INF ) return intersection;
	
	intersection = geo_util::segmentSegmentIntersectionPoint(a, b, Point(x1, y1), Point(x2, y1));
	if(intersection.get<0>() != INF and intersection.get<1>() != INF ) return intersection;
	
	intersection = geo_util::segmentSegmentIntersectionPoint(a, b, Point(x2, y1), Point(x2, y2));
	if(intersection.get<0>() != INF and intersection.get<1>() != INF ) return intersection;
	
	intersection = geo_util::segmentSegmentIntersectionPoint(a, b, Point(x1, y2), Point(x2, y2));
	if(intersection.get<0>() != INF and intersection.get<1>() != INF ) return intersection;
	return b;
}

vector<Point> levyFlight(vector<Point>& cuckoos, Point bestCuckoo, long double x1, long double x2, long double y1, long double y2)
{
	assert( geo_util::dblcmp(x1 - bestCuckoo.get<0>()) <= 0 && geo_util::dblcmp(bestCuckoo.get<0>() - x2) <= 0 );
	assert( geo_util::dblcmp(y1 - bestCuckoo.get<1>()) <= 0 && geo_util::dblcmp(bestCuckoo.get<1>() - y2) <= 0 );
	vector<Point> newCuckoos(cuckoos.size());

	long double beta = 1.5;
	long double sigma = (tgamma(1 + beta)* sin(PI * beta / 2) / (tgamma((1 + beta) / 2) * beta * pow(2.0, (beta - 1) / 2)));
	long double sigmaPow = pow(sigma, 1 / beta);

	for(int i = 0; i < cuckoos.size(); i += 1)
	{
		Point s = cuckoos[ i ], u, v, step;
		u = normalDistribution(sigmaPow, true);
		v = normalDistribution(sigmaPow, false);
		
		// mantegna algorithm
		step = Point(u.get<0>() / pow(abs(v.get<0>()), 1.0 / beta),
					 u.get<1>() / pow(abs(v.get<1>()), 1.0 / beta));

		step = Point( 
			0.01 * step.get<0>() * (s.get<0>() - bestCuckoo.get<0>()),
			0.01 * step.get<1>() * (s.get<0>() - bestCuckoo.get<1>())
		);
		// random walk
		Point walk = normalDistribution(sigmaPow, false);
		long double x = s.get<0>() + step.get<0>() * walk.get<0>();
		long double y = s.get<1>() + step.get<1>() * walk.get<1>();

		
		if( x < x1 ) x = x1;
		else if( x > x2 ) x = x2;
		if(y < y1) y = y1;
		else if( y > y2 ) y = y2;
		
		newCuckoos[i] = Point(x, y);
	}

	return newCuckoos;
}

void sortCuckoosByEval(MultiPolygon& packing, int polygonId, long double rotationAngle, vector<Point> &cuckoos, bool increasing)
{
	vector<std::tuple<long double, int>> forSorting;
	for(int i = 0; i < NUMBER_OF_CUCKOOS; i += 1)
	{
		long double eval = evaluate(packing, polygonId, cuckoos[i], rotationAngle);
		if( increasing == false ) eval = -eval; 
		forSorting.push_back({eval, i});
	}
	sort(forSorting.begin(), forSorting.end());
	vector<Point> __cuckoos = cuckoos; 
	cuckoos.clear();

	for(int i = 0; i < NUMBER_OF_CUCKOOS; i += 1)
	{
		long double eval;
		int index;
		std::tie(eval, index) = forSorting[i];
		cuckoos.push_back(__cuckoos[index]);
	}
}

Polygon translatePolygon(Polygon polygon, Point point)
{
	Point reference = polygon.outer()[0];
	boost_geo::multiply_value(reference, -1);
	polygon = geo_util::poly_util::translate(polygon, reference);
	polygon = geo_util::poly_util::translate(polygon, point);
	return polygon;
}

/**Point bin_packing::cuckooSearch( MultiPolygon &packing, vector<vector<long double>> &penalty, int polygonId, long double rotationAngle, long double length, long double width)
{
	Polygon polygon = packing[polygonId];
	polygon = geo_util::poly_util::rotateCW(polygon, rotationAngle, polygon.outer()[0]);
	Polygon ifr = polygon_fit::getInnerFitRectangle({polygon}, length, width);

	double x1 = ifr.outer()[0].get<0>();
	double y1 = ifr.outer()[0].get<1>();
	double x2 = ifr.outer()[2].get<0>();
	double y2 = ifr.outer()[2].get<1>();

	srand(time(NULL));
	vector<Point> cuckoos(NUMBER_OF_CUCKOOS);
	for (int i = 0; i < NUMBER_OF_CUCKOOS; i += 1)
	{
		cuckoos[i] = getCuckoo(x1, x2, y1, y2);
		assert( x1 <= cuckoos[i].get<0>() && cuckoos[i].get<0>() <= x2 );
		assert( y1 <= cuckoos[i].get<1>() && cuckoos[i].get<1>() <= y2 );
	}
	
	Point bestCuckoo = cuckoos[0];
	int t = 0;

	while( t < MAXIMUM_NUMBER_OF_GENERATION )
	{
		int randCuckooIndexA = rand() % NUMBER_OF_CUCKOOS;
		Point randCuckooA = cuckoos[randCuckooIndexA];
		randCuckooA = levyFlight(cuckoos, randCuckooA, x1, x2, y1, y2)[0];
		long double evalA = evaluate(packing, polygonId, randCuckooA, rotationAngle);

		int randCuckooIndexB = rand() % NUMBER_OF_CUCKOOS;
		Point randCuckooB = cuckoos[randCuckooIndexB];
		long double evalB = evaluate(packing, polygonId, randCuckooB, rotationAngle);

		if( geo_util::dblcmp(evalA - evalB) < 0 )
		{
			cuckoos[randCuckooIndexB] = randCuckooA;
			bestCuckoo = randCuckooB;
		}

		sortCuckoosByEval(packing, polygonId, rotationAngle, cuckoos, false);
		int ABANDON_NESTS = DISCOVER_PROBABILITY * NUMBER_OF_CUCKOOS;
		vector<Point> newCuckoos = levyFlight(cuckoos, bestCuckoo, x1, x2, y1, y2);

		for(int i = 0; i < ABANDON_NESTS; i += 1)
		{
			cuckoos[i] = newCuckoos[i];
		}
		sortCuckoosByEval(packing, polygonId, rotationAngle, cuckoos, true);
		bestCuckoo = cuckoos[0];
		t += 1;
	}
	assert(geo_util::dblcmp(x1 - bestCuckoo.get<0>()) <= 0 && geo_util::dblcmp(y1 - bestCuckoo.get<1>()) <= 0);
	assert(geo_util::dblcmp(x2 - bestCuckoo.get<0>()) >= 0 && geo_util::dblcmp(y2 - bestCuckoo.get<1>()) >= 0);
	//std::cout << "      " << "Cuckoo: " << boost_geo::wkt(bestCuckoo) << " " << x1 << " " << x2 << " " << y1 << " " << y2 << "\n";
	//std::cout << "      " << "length: " << length << " " << width << std::endl;
	temp.push_back(translatePolygon(polygon, bestCuckoo));
	//geo_util::visualize(temp, "../io/results/dighe1/gurbage/", "ifr");
	return bestCuckoo;
}
**/


long double findBestCuckoo(Point &bestCuckoo, vector<Point> &cuckoos, vector<Point> &newCuckoos, vector<long double> &fitnesses, MultiPolygon& packing, int polygonId, long double rotationAngleInDegree)
{
	int n = cuckoos.size();
	for(int i = 0; i < n; i += 1)
	{
		long double fitness = evaluate(packing, polygonId, newCuckoos[i], rotationAngleInDegree);
		if( fitness < fitnesses[i] )
		{
			fitnesses[i] = fitness;
			cuckoos[i] = newCuckoos[i]; 
		}
	}

	//for(int i = 0; i < n; i += 1) std::cout << boost_geo::wkt(cuckoos[i]) << " *\n";
	//for(int i = 0; i < n; i += 1) std::cout << fitnesses[i] << " -\n";

	double minFitness = INF;
	for(int i = 0; i < n; i += 1)
	{
		if( minFitness > fitnesses[i])
		{
			minFitness = fitnesses[i];
			bestCuckoo = cuckoos[i];
		}
	}
	// std::cout << boost_geo::wkt(bestCuckoo) << " best cuckoo\n";
	return minFitness;
}

long double getRandomValue(long double l, long double r)
{
	long double randValue = (long double)rand() / RAND_MAX;
	return l + randValue * (r - l);
}

vector<Point> emptyCuckoos(vector<Point> &cuckoos, long double x1, long double x2, long double y1, long double y2)
{
	int n = cuckoos.size();
	unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed2);

	vector<vector<long double>> k(n, vector<long double>(2));
	for(int i = 0; i < n; i += 1)
	{
		k[i][0] = ( getRandomValue(-1, 1) > DISCOVER_PROBABILITY) ? 1.0 : 0.0;
		k[i][1] = ( getRandomValue(-1, 1) > DISCOVER_PROBABILITY) ? 1.0 : 0.0;
	}

	unsigned seed3 = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed3);
	vector<Point> randCuckoos1(n), randCuckoos2(n);
	vector<vector<long double>> stepSize(n, vector<long double>(2));
	for (int i = 0; i < n; i += 1)
	{
		randCuckoos1[i] = getCuckoo(x1, x2, y1, y2);
		randCuckoos2[i] = getCuckoo(x1, x2, y1, y2);
		stepSize[i][0] = getRandomValue(-1.0, 1.0) * (randCuckoos1[i].get<0>() - randCuckoos2[i].get<0>());
		stepSize[i][1] = getRandomValue(-1.0, 1.0) * (randCuckoos1[i].get<1>() - randCuckoos2[i].get<1>());
	}
	vector<Point> newCuckoos(n);
	for(int i = 0; i < n; i += 1)
	{
		long double x = cuckoos[i].get<0>() + stepSize[i][0] * k[i][0];
		long double y = cuckoos[i].get<1>() + stepSize[i][1] * k[i][1];

		if( x < x1 ) x = x1;
		else if( x > x2 ) x = x2;

		if(y < y1) y = y1;
		else if( y > y2 ) y = y2;

		newCuckoos[i] = Point(x, y);
		assert(geo_util::dblcmp(x1 - newCuckoos[i].get<0>()) <= 0 && geo_util::dblcmp(newCuckoos[i].get<0>() - x2) <= 0);
		assert(geo_util::dblcmp(y1 - newCuckoos[i].get<1>()) <= 0 && geo_util::dblcmp(newCuckoos[i].get<1>() - y2) <= 0);
	}
	return newCuckoos;
}
Point bin_packing::cuckooSearch( MultiPolygon &packing, vector<vector<long double>> &penalty, int polygonId, long double rotationAngle, long double length, long double width)
{
	Polygon polygon = packing[polygonId];
	polygon = geo_util::poly_util::rotateCW(polygon, rotationAngle, polygon.outer()[0]);
	Polygon ifr = polygon_fit::getInnerFitRectangle({polygon}, length, width);

	double x1 = ifr.outer()[0].get<0>();
	double y1 = ifr.outer()[0].get<1>();
	double x2 = ifr.outer()[2].get<0>();
	double y2 = ifr.outer()[2].get<1>();

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	srand(seed1);
	vector<Point> cuckoos(NUMBER_OF_CUCKOOS);
	for (int i = 0; i < NUMBER_OF_CUCKOOS; i += 1)
	{
		cuckoos[i] = getCuckoo(x1, x2, y1, y2);
		assert( x1 <= cuckoos[i].get<0>() && cuckoos[i].get<0>() <= x2 );
		assert( y1 <= cuckoos[i].get<1>() && cuckoos[i].get<1>() <= y2 );
	}

	vector<long double> fitnesses(NUMBER_OF_CUCKOOS, INF);
	Point bestCuckoo;
	long double minFitness = findBestCuckoo(bestCuckoo, cuckoos, cuckoos, fitnesses, packing, polygonId, rotationAngle);

	long double tolerance = 1e-4;
	int iter = 0;
	while( minFitness > tolerance && iter < MAXIMUM_NUMBER_OF_GENERATION)
	{
		//std::cout << boost_geo::wkt(bestCuckoo) << " " << x1 << " " << x2 << " " << y1 << " " << y2 << " enter\n";
		vector<Point> newCuckoos = levyFlight(cuckoos, bestCuckoo, x1, x2, y1, y2);
		Point best;
		//std::cout << boost_geo::wkt(bestCuckoo) << " " << x1 << " " << x2 << " " << y1 << " " << y2 << " levy\n";
		long double fitness = findBestCuckoo(best, cuckoos, newCuckoos, fitnesses, packing, polygonId, rotationAngle);
		newCuckoos = emptyCuckoos(cuckoos, x1, x2, y1, y2);
		fitness = findBestCuckoo(best, cuckoos, newCuckoos, fitnesses, packing, polygonId, rotationAngle);
		//std::cout << boost_geo::wkt(bestCuckoo) << " " << x1 << " " << x2 << " " << y1 << " " << y2 << " empty\n";
		if( fitness < minFitness )
		{
			minFitness = fitness;
			bestCuckoo = best;
			//std::cout << minFitness << " " << boost_geo::wkt(best) << "\n";
		}
		// std::cout << boost_geo::wkt(bestCuckoo) << " " << x1 << " " << x2 << " " << y1 << " " << y2 << " exit\n";
		iter += 1;
	}
	assert(geo_util::dblcmp(x1 - bestCuckoo.get<0>()) <= 0 && geo_util::dblcmp(bestCuckoo.get<0>() - x2) <= 0);
	assert(geo_util::dblcmp(y1 - bestCuckoo.get<1>()) <= 0 && geo_util::dblcmp(bestCuckoo.get<1>() - y2) <= 0);
	return bestCuckoo;
}
std::tuple<MultiPolygon, bool> bin_packing::minimizeOverlap(MultiPolygon packing, vector<long double> allowableRoatations, long double width, long double length,long double totalAreaOfInputPolygons, std::ofstream &outputLog, string outputLocation)
{
	long double fitness = INF;
	int iter = 0;
	int n = packing.size();
	
	vector<vector<long double>> penalty(n, vector<long double>(n, 1.0));
	vector<vector<long double>> depths(n, vector<long double> (n, 0.0));
	while( iter < MAXIMUM_ITERATIONS_FOR_LOCAL_MINIMA )
	{
		std::cout << "    minimizeOverlap iteration(" << iter << ")\n";
		std::cout << "    length: " << length << "\n";
		std::cout << "    width: " << width << "\n";
		std::cout << "    total area: " << totalAreaOfInputPolygons << "\n";
		vector<int> Q;
		for(int i = 0; i < n; i += 1) 
		{
			Q.push_back(i);
		}
		random_shuffle(Q.begin(), Q.end());
		for(int i = 0; i < n; i += 1)
		{
			Point v = packing[Q[i]].outer()[0];
			long double o = 0;
			long double eval = evaluate(packing, Q[i], v, o);

			for(auto rotation: allowableRoatations)
			{
				Point cuckoo = bin_packing::cuckooSearch(packing, penalty, Q[i], rotation, length, width);
				long double currentEval = evaluate(packing, Q[i], cuckoo, rotation);
				if( geo_util::dblcmp(currentEval - eval) < 0 )
				{
					eval = currentEval;
					v = cuckoo;
					o = rotation;
				}
			}
			std::cout << "      Polygon (" << i << "/" << n << ")" << " eval => " << eval << "\n";
			packing[Q[i]] = rotateTranslate(packing[Q[i]], v, o);
			// std::cout << "      " << boost_geo::wkt(packing[Q[i]]) << "\n";
			// std::cout << "      " << geo_util::poly_util::getWidth(packing) << "*****\n";
		}

		geo_util::visualize(packing, "../io/results/dighe1/gurbage/", "bug");
		long double currentFitness = evaluateAll(packing, penalty, depths);
		std::cout << "    => current fitness: " << currentFitness << "\n";
		std::cout << "    => packing length.: " << geo_util::poly_util::getLength(packing) << "\n";
		std::cout << "    => packing width..: " << geo_util::poly_util::getWidth(packing) << "\n";
		std::cout << "    => area...........: " << boost_geo::area(packing) << "\n";
		if( geo_util::dblcmp(currentFitness - FEASIBILTY) <= 0 )
		{
			return {packing, true};
		}
		else if( geo_util::dblcmp(currentFitness - fitness) < 0 )
		{
			fitness = currentFitness;
			iter = -1;
		}
		// bin_packing::increasePenalty(packing, penalty, depths);
		iter += 1;
	}
	return {packing, false};
}

void bin_packing::cuckooPacking(string datasetname, string outputLocation, long double width, long double runTimeDuration)
{
	// clock started
	auto start = std::chrono::high_resolution_clock::now();
	std::time_t start_time = std::chrono::system_clock::to_time_t(start);

	// create directory for cuckoo packing results
	string cuckooPackingDirectoryName = outputLocation + "/" + datasetname + "/cuckoo_packing" + std::ctime(&start_time);
	mkdir(cuckooPackingDirectoryName.c_str(), 0777);
	// create directory for the svg files representing packing steps
	string cuckooPackingStepsDirectoryName = cuckooPackingDirectoryName + "/steps";
	mkdir(cuckooPackingStepsDirectoryName.c_str(), 0777);
	// create or open logger file
	std::ofstream outputLog(cuckooPackingDirectoryName + "/" + "output_log.txt",
							std::ios_base::out | std::ios_base::app);
	outputLog << std::fixed << std::setprecision(3);
	outputLog << "start at.....................................: "
			  << std::ctime(&start_time) << std::endl;

	outputLog << "reading initial packing......................:" << std::endl;
	string initialSolutionFileName = outputLocation + "/" + datasetname + "/initial_solution/initial_packing.wkt";
	MultiPolygon initialPacking;
	geo_util::poly_util::readWKTMultiPolygon(initialPacking, initialSolutionFileName);
	outputLog << boost_geo::wkt(initialPacking) << std::endl
			  << std::endl;

	geo_util::visualize(initialPacking, cuckooPackingDirectoryName, "initial_packing");
	outputLog << "initial packing density......................: "
			  << bin_packing::getPackingDensity(initialPacking) << std::endl;

	outputLog << "running cuckooPacking()......................: " << std::endl;

	long double totalAreaOfInitialPackingPolygons = 0;
	for (Polygon &polygon : initialPacking)
	{
		totalAreaOfInitialPackingPolygons += std::fabs(boost_geo::area(polygon));
	}

	MultiPolygon bestPacking = initialPacking;
	MultiPolygon currentPacking = initialPacking;
	MultiPolygon originalPacking = initialPacking;

	long double bestLength = geo_util::poly_util::getLength(initialPacking);
	long double decreasingRate = LENGTH_DECREASING_RATE;
	long double increasingRate = LENGTH_INCREASING_RATE;
	long double currentLength = (1 - decreasingRate) * bestLength;
	int feasiblePackingId = 0;

	pushDown(currentPacking, currentLength);
	int iter = 3;
	while (iter > 0)
	{

		std::cout << "Iteration( " << iter << " )" << std::endl;
		bool flag;
		std::tie(currentPacking, flag) = bin_packing::minimizeOverlap(currentPacking, ALLOWABLE_ROTATIONS, width, currentLength, totalAreaOfInitialPackingPolygons, outputLog, cuckooPackingStepsDirectoryName);
		if (flag == true)
		{
			long double currentAccuracy = bin_packing::getPackingDensity(currentPacking);
			outputLog << "feasible packing id..........................: " << feasiblePackingId << std::endl;
			outputLog << "current accuracy.............................: " << currentAccuracy << std::endl;
			bestPacking = currentPacking;
			bestLength = currentLength;
			currentLength = (1.0 - decreasingRate) * currentLength;
			pushDown(currentPacking, currentLength);

			std::ofstream feasiblePackingWKTFile(cuckooPackingDirectoryName + "/feasible_packing_" + std::to_string(feasiblePackingId) + ".wkt");
			feasiblePackingWKTFile << boost_geo::wkt(bestPacking) << std::endl;
			feasiblePackingWKTFile.close();
			geo_util::visualize(bestPacking, cuckooPackingDirectoryName, "feasible_packing_" + std::to_string(feasiblePackingId++));
		}
		else
		{
			currentPacking = originalPacking;
			currentLength = (1.0 + increasingRate) * currentLength;
			iter -= 1;
		}
		geo_util::visualize(currentPacking, cuckooPackingStepsDirectoryName, "minimized_overlap");
	}

	std::ofstream finalPackingWKTFile(cuckooPackingDirectoryName + "/final_packing.wkt");
	finalPackingWKTFile << boost_geo::wkt(bestPacking) << std::endl;
	finalPackingWKTFile.close();

	outputLog << "final packing density........................: "
			  << bin_packing::getPackingDensity(bestPacking) << std::endl;

	outputLog << std::endl;
	outputLog << "final packing ...............................: " << std::endl;
	outputLog << boost_geo::wkt(bestPacking) << std::endl;
	geo_util::visualize(bestPacking, cuckooPackingDirectoryName, "final_packing");

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	outputLog << "time taken by function.......................: "
			  << duration.count() / 1000000.0 << " seconds" << std::endl
			  << std::endl;

	std::time_t stop_time = std::chrono::system_clock::to_time_t(stop);
	outputLog << "terminated at................................: "
			  << std::ctime(&stop_time) << std::endl;

	outputLog << "-----------------------------------------------------------------------------------"
			  << std::endl
			  << std::endl;
	outputLog.close();
}
