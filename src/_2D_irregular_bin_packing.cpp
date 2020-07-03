#include <_2D_irregular_bin_packing.hpp>

/**
 *  polygon to string
*/
string dnc_approach::toStringPoly(Polygon poly)
{
    string str = "POLYGON((";
    for (auto point : poly)
    {
        str += to_string(point.X);
        str += " ";
        str += to_string(point.Y);
        str += ", ";
    }
    str += to_string(poly[0].X);
    str += " ";
    str += to_string(poly[0].Y);
    str += "))";
    return str;
}

/**
 *  prints a polygon
*/
void dnc_approach::printPolygon(Polygon poly)
{
    cout << "Polygon: "
         << "\n";
    for (auto point : poly)
    {
        cout << setprecision(2) << fixed << point.X << " " << point.Y << endl;
    }
}

/**
 *  union of two polygon
*/
Polygon dnc_approach::polygonUnion(Polygon poly1, Polygon poly2)
{
    Polygon verybig({{0, 0}, {0, INF}, {INF, INF}});
    if (poly1.size() == 0 && poly2.size() == 0)
    {
        return verybig;
    }
    if (poly1.size() == 0)
    {
        return poly2;
    }
    if (poly2.size() == 0)
    {
        return poly1;
    }
    Polygon poly;
    assert(poly1.size() > 0);
    assert(poly2.size() > 0);
    polygon green, blue;
    boost::geometry::read_wkt(toStringPoly(poly1), green);
    boost::geometry::read_wkt(toStringPoly(poly2), blue);
    std::vector<polygon> output;
    boost::geometry::union_(green, blue, output);
    BOOST_FOREACH (polygon const &p, output)
    {
        for (int i = 0; i < p.outer().size(); i += 1)
        {
            double _x = p.outer().at(i).x();
            double _y = p.outer().at(i).y();
            poly.emplace_back(_x, _y);
        }
    }

    return poly;
}

/**
 *  returns the min corner and max corner of the axis aligned bounding rectangle
*/
double dnc_approach::minBoundingRectangle(Polygon poly)
{
    if (poly.size() == 0)
    {
        return INF;
    }
    Point a, b;
    assert(poly.size() > 0);
    boost::geometry::model::polygon<point> polygon;
    boost::geometry::read_wkt(toStringPoly(poly), polygon);
    boost::geometry::model::box<point> box;
    boost::geometry::envelope(polygon, box);
    point p = box.min_corner();
    point q = box.max_corner();
    a.X = p.get<0>();
    a.Y = p.get<1>();
    b.X = q.get<0>();
    b.Y = q.get<1>();

    // cout << a.X << " " << a.Y << " " << b.X << " " << b.Y << endl;
    return (b.X - a.X) * (b.Y - a.Y);
}

/**
 *  area of a polygon
*/
double dnc_approach::areaPoly(Polygon poly)
{
    if (poly.size() == 0)
    {
        return INF;
    }
    assert(poly.size() > 0);
    boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double>> p;
    boost::geometry::read_wkt(toStringPoly(poly), p);
    return boost::geometry::area(p);
}

/**
 * rotate point p around point o, angle degree
 * angle < 0 clockwise rotation, otherwise anticlockwise
*/
Point dnc_approach::rotatePoint(Point p, Point o, double angle)
{
    angle = (angle * (acos(-1.0) / 180.0));
    Point v = {p.X - o.X, p.Y - o.Y};
    double c = cos(angle), s = sin(angle);
    return {o.X + v.X * c - v.Y * s, o.Y + v.X * s + v.Y * c};
}

/**
 * rotates polygon poly around point o, angle degree
 * angle < 0 clockwise rotation, otherwise anticlockwise
*/
Polygon dnc_approach::rotatePolygon(Polygon poly, Point o, double angle)
{
    if (poly.size() == 0)
    {
        return poly;
    }
    for (auto &point : poly)
    {
        point = rotatePoint(point, o, angle);
    }
    return poly;
}

/**
 * reflection of points to a line y = mx + b
*/
Polygon dnc_approach::reflectAcrossLine(Polygon poly, Point p, Point q)
{
    double x1 = p.X, x2 = q.X;
    double y1 = p.Y, y2 = q.Y;

    if ((x2 - x1) < EPS)
    {
        for (auto &point : poly)
        {
            point.X = 2 * x1 - point.X;
        }
    }
    else if ((y2 - y1) < EPS)
    {
        for (auto &point : poly)
        {
            point.Y = 2 * y1 - point.Y;
        }
    }
    else
    {
        double m = (y2 - y1) / (x2 - x1);
        double b = y1 - m * x1;

        for (auto &point : poly)
        {
            double x = point.X;
            double y = point.Y;
            point.X = ((1 - m * m) * x + 2 * m * y - 2 * m * b) / (m * m + 1);
            point.Y = ((m * m - 1) * y + 2 * m * x + 2 * b) / (m * m + 1);
        }
    }
    return poly;
}

/** 
 * finds the angle in radian between pq and rs
*/
double dnc_approach::findRotationAngle(Point pq, Point rs)
{
    double angle = atan2(pq.Y, pq.X) - atan2(rs.Y, rs.X);
    if (angle < 0)
        angle += 2.0 * PI;
    return angle;
}

/**
 * let say p, q are the endpoints of Item A's largest edge
 * let say r, s are the endpoints of Item B's largest edge
 * we want to place Item B's point r at point p and return the resultant Item b
*/
Polygon dnc_approach::placement(Polygon poly, Point p, Point q, Point r, Point s)
{
    double angle = findRotationAngle({q.X - p.X, q.Y - p.Y}, {s.X - r.X, s.Y - r.Y});
    Point _p = {0, 0};
    Point _q = {q.X - p.X, q.Y - p.Y};
    Point _r = {0, 0};
    Point _s = {s.X - r.X, s.Y - r.Y};
    for (auto &point : poly)
    {
        point = rotatePoint(point, r, angle);
        point = {point.X - r.X, point.Y - r.Y};
        point = {point.X + p.X, point.Y + p.Y};
    }
    return poly;
}

/**
 * returns the itersection area of two polygons
*/
double dnc_approach::polygonPolygonIntersectionArea(Polygon poly1, Polygon poly2)
{
    double area = 0.0;
    polygon green, blue;
    boost::geometry::read_wkt(toStringPoly(poly1), green);
    boost::geometry::read_wkt(toStringPoly(poly2), blue);
    std::deque<polygon> output;
    boost::geometry::intersection(green, blue, output);
    BOOST_FOREACH (polygon const &p, output)
    {
        area += boost::geometry::area(p);
    }
    return area;
}

Polygon dnc_approach::normalize(Polygon poly)
{
    assert(poly.size() > 0);
    double _x = INF;
    double _y = INF;

    for (auto point : poly)
    {
        _x = min(_x, point.X);
        _y = min(_y, point.Y);
    }

    for (auto &point : poly)
    {
        point.X -= _x;
        point.Y -= _y;
    }
    return poly;
}

/**
 *  read from file
*/
void dnc_approach::readIO(string filename, vector<Polygon> &polygons, vector<int> &quantity, double &width)
{
    ifstream file(filename);
    if (file.fail())
    {
        cerr << "File does not exists\n";
    }
    else
    {
        int n;
        file >> width;
        file >> n;
        quantity.resize(n);
        for (int i = 0; i < n; i += 1)
        {
            Polygon poly, _poly;
            file >> quantity[i];

            // reading polygon counter clockwise as given in the dataset
            int m;
            file >> m;
            while (m--)
            {
                double _x, _y;
                file >> _x >> _y;
                _poly.push_back({_x, _y});
            }

            // make clockwise
            poly.push_back(_poly[0]);
            for (int j = _poly.size() - 1; j > 0; j -= 1)
                poly.push_back(_poly[j]);

            poly = normalize(poly);
            //printPolygon(poly);
            for (int k = 0; k < quantity[i]; k += 1)
                polygons.push_back(poly);
        }
    }
    file.close();
}

void dnc_approach::processVisualize(vector<Polygon> polygons)
{
    vector<Polygon> temp = polygons;
    polygons.clear();
    for (auto _poly : temp)
    {
        Polygon poly;
        poly.push_back(_poly[0]);
        for (int j = _poly.size() - 1; j > 0; j -= 1)
            poly.push_back(_poly[j]);
        polygons.push_back(poly);
    }

    double _x = INF, _y = INF;
    for (auto &poly : polygons)
    {
        for (auto &point : poly)
        {
            _x = std::min(_x, point.X);
            _y = std::min(_y, point.Y);
        }
    }
    for (auto &poly : polygons)
    {
        for (auto &point : poly)
        {
            point.X -= _x;
            point.Y -= _y;
        }
    }
    _x = -INF, _y = -INF;
    for (auto &poly : polygons)
    {
        for (auto &point : poly)
        {
            _x = std::max(_x, point.X);
            _y = std::max(_y, point.Y);
        }
    }

    _x = 800 / _x;
    _y = 600 / _y;

    for (auto &poly : polygons)
    {
        for (auto &point : poly)
        {
            point.X *= _x;
            point.Y *= _y;
        }
    }

    cout << "[";
    for (int i = 0; i < polygons.size(); i += 1)
    {
        Polygon poly = polygons[i];
        cout << "[" << poly[0].X << ", " << poly[0].Y;
        for (int j = 1; j < poly.size(); j += 1)
        {
            Point point = poly[j];
            cout << setprecision(2) << fixed << ", " << point.X << ", " << point.Y;
        }
        cout << setprecision(2) << fixed << ", " << poly[0].X << ", " << poly[0].Y << "]\n";
        if (i < polygons.size() - 1)
            cout << ",";
    }
    cout << "]\n";
}

// ---------------------------------ALGORITHM AREA ----------------------------------
tuple<vector<Polygon>, Polygon> dnc_approach::Translate(tuple<vector<Polygon>, Polygon> A, Point p)
{
    vector<Polygon> polygons;
    Polygon poly;

    tie(polygons, poly) = A;
    for (auto &_poly : polygons)
    {
        for (auto &point : _poly)
        {
            point.X += p.X;
            point.Y += p.Y;
        }
    }
    for (auto &point : poly)
    {
        point.X += p.X;
        point.Y += p.Y;
    }
    return {polygons, poly};
}

tuple<vector<Polygon>, Polygon> dnc_approach::Rotate(tuple<vector<Polygon>, Polygon> A, Point o, double ang)
{
    vector<Polygon> polygons;
    Polygon poly;
    tie(polygons, poly) = A;

    for (auto &_poly : polygons)
    {
        _poly = rotatePolygon(_poly, o, ang);
    }
    for (auto &point : poly)
    {
        point = rotatePoint(point, o, ang);
    }
    return {polygons, poly};
}

tuple<vector<Polygon>, Polygon> dnc_approach::Normalize(tuple<vector<Polygon>, Polygon> A)
{
    vector<Polygon> polygons;
    Polygon poly;
    tie(polygons, poly) = A;

    double _x = INF, _y = INF;
    for (auto &_poly : polygons)
    {
        for (auto &point : _poly)
        {
            _x = min(_x, point.X);
            _y = min(_y, point.Y);
        }
    }
    for (auto &point : poly)
    {
        _x = min(_x, point.X);
        _y = min(_y, point.Y);
    }

    for (auto &_poly : polygons)
    {
        for (auto &point : _poly)
        {
            point.X -= _x;
            point.Y -= _y;
        }
    }
    for (auto &point : poly)
    {
        point.X -= _x;
        point.Y -= _y;
    }
    return {polygons, poly};
}

bool dnc_approach::doesOverlap(tuple<vector<Polygon>, Polygon> A, tuple<vector<Polygon>, Polygon> B)
{
    double area = 0;
    for (auto poly1 : get<0>(A))
    {
        for (auto poly2 : get<0>(B))
        {
            area += polygonPolygonIntersectionArea(poly1, poly2);
            if (area > 0)
                return true;
        }
    }
    return false;
}

tuple<vector<Polygon>, Polygon> dnc_approach::merge(tuple<vector<Polygon>, Polygon> A, tuple<vector<Polygon>, Polygon> B)
{
    if (get<1>(A).size() == 0)
    {
        return B;
    }
    if (get<1>(B).size() == 0)
    {
        return A;
    }
    tuple<vector<Polygon>, Polygon> best;
    double minMBR = INF;

    double area1 = areaPoly(get<1>(A));
    double area2 = areaPoly(get<1>(B));
    if (area1 < area2)
    {
        swap(A, B);
    }
    for (auto point : get<1>(A))
    {
        tuple<vector<Polygon>, Polygon> C = Translate(B, point);
        // overlap check
        if (doesOverlap(A, C) > 0)
        {
            continue;
        }
        Polygon united = polygonUnion(get<1>(A), get<1>(C));
        double MBR = minBoundingRectangle(united);
        if (MBR < minMBR)
        {
            minMBR = MBR;
            get<1>(best) = united;
            get<0>(best) = get<0>(A);
            for (auto poly : get<0>(C))
            {
                get<0>(best).push_back(poly);
            }
        }
    }
    return best;
}

tuple<vector<Polygon>, Polygon> dnc_approach::split(vector<Polygon> &polygons, int l, int r)
{
    if (l == r)
    {
        //polygons[ l ] = normalize(polygons[ l ]);
        return {{polygons[l]}, polygons[l]};
    }
    int mid = (l + r) >> 1;

    tuple<vector<Polygon>, Polygon> A = split(polygons, l, mid);
    tuple<vector<Polygon>, Polygon> B = split(polygons, mid + 1, r);

    tuple<vector<Polygon>, Polygon> best;
    double minMBR = INF;

    for (int i = 0; i < 360; i += 90)
    {
        tuple<vector<Polygon>, Polygon> C = Rotate(A, {0, 0}, i);
        C = Normalize(C);
        for (int j = 0; j < 360; j += 90)
        {
            tuple<vector<Polygon>, Polygon> D = Rotate(B, {0, 0}, j);
            D = Normalize(D);
            tuple<vector<Polygon>, Polygon> E = merge(C, D);
            double MBR = minBoundingRectangle(get<1>(E));
            if (MBR < minMBR)
            {
                minMBR = MBR;
                best = E;
            }
        }
    }
    return best;
}