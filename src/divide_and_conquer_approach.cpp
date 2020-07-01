#include "divide_and_conquer_approach.hpp"

/**
 * returns 1 if left, -1 otherwise
*/
int approach1::orient(Point a, Point b, Point c)
{
    int p = (b - a).determinant(c - a);
    return p > 0 ? 1 : -1;
}

/**
 *finds the largest edge of an item
*/
std::pair<Point, Point> approach1::findLargestEdge(Item &item)
{
    int noOfVertices = item.vertices.size();
    double d = 0;
    Point p, q;

    for (int i = 0; i < noOfVertices; i += 1)
    {
        Point _p = item.vertices[i];
        Point _q = item.vertices[(i + 1) % noOfVertices];
        double _d = (_p.x - _q.x) * (_p.x - _q.x) + (_p.y - _q.y) * (_p.y - _q.y);
        if (_d > d)
        {
            d = _d;
            p = _p;
            q = _q;
        }
    }
    return {p, q};
}

/** 
 * finds the angle in radian between pq and rs
*/
double approach1::findRotationAngle(Point pq, Point rs)
{
    double lengthOfPQ = sqrt((pq.x * pq.x) + (pq.y * pq.y));
    double lengthOfRS = sqrt((rs.x * rs.x) + (rs.y * rs.y));
    double angle = acos(((pq.x * rs.x) + (pq.y * rs.y)) / (lengthOfPQ * lengthOfRS));
    return angle;
}

/**
 * let say p, q are the endpoints of Item A's largest edge
 * let say r, s are the endpoints of Item B's largest edge
 * we want to place Item B's point r at point p and return the resultant Item b
*/
Item approach1::placement(Item item, Point p, Point q, Point r, Point s)
{
    double angle = findRotationAngle(q - p, s - r);
    int isLeft = orient(r, s, q);
    for (auto &point : item.vertices)
    {
        point = point.rotate(r, angle * isLeft);
        point = point - r;
        point = point + p;
    }
    return item;
}

/**
 * returns the outerface of the Point a, and b after placing 
 * Point b's point r at Point a's point p
*/
Polygon approach1::outerface(Polygon &polygon1, Polygon &polygon2, int id1, int id2)
{
    int n = polygon1.size();
    int m = polygon2.size();
    assert(n >= 3 and m >= 3 and id1 < n and id2 < m);
    Polygon mergedPolygon(polygon1.begin(), polygon1.begin() + id1 + 1);
    int id = (id2 + 1) % m;
    for (int k = id2 - 1;; k--)
    {
        if (k < 0)
        {
            k = m - 1;
        }
        mergedPolygon.push_back(polygon2[k]);
        if (k == id)
        {
            break;
        }
    }
    id1 = (id1 + 1) % n;
    if (polygon2[id] != polygon1[id1])
    {
        mergedPolygon.push_back(polygon1[id1]);
    }
    if (id1 < n)
    {
        for (int i = id1; i < n; i++)
        {
            mergedPolygon.push_back(polygon1[i]);
        }
    }
    return mergedPolygon;
}

/**
 * returns the min bounding rectangle axis parallel
*/
std::pair<double, double> approach1::minAreaRectangle(Polygon &polygon)
{
    double min_x = INF, max_x = -INF;
    double min_y = INF, max_y = -INF;
    for (Point vertex : polygon)
    {
        min_x = std::min(min_x, vertex.x);
        min_y = std::min(min_y, vertex.y);
        max_x = std::max(max_x, vertex.x);
        max_y = std::max(max_y, vertex.y);
    }
    double l = max_x - min_x;
    double w = max_y - min_y;
    return {l, w};
}

/**
 * reflection of points to a line y = mx + b
*/
Item approach1::reflectAcrossLine(Item item, Point p, Point q)
{

    double x1 = p.x, x2 = q.x;
    double y1 = p.y, y2 = q.y;
    assert(fabs(x2 - x1) < geo::EPS);
    double m = (y2 - y1) / (x2 - x1);
    double b = y1 - m * x1;
    for (auto &point : item.vertices)
    {
        point.x = ((1 - m * m) * point.x + 2 * m * point.y - 2 * m * b) / (m * m + 1);
        point.y = ((m * m - 1) * point.y + 2 * m * point.x + 2 * b) / (m * m + 1);
    }
    return item;
}
/**
 * merges two item by proposed heuristic1
*/
Item approach1::mergeHeuristic1(Item &item1, Item &item2)
{
    Item mergedItem;
    double minArea = INF;

    auto checkForOptimal = [&](Polygon &polygon1, Polygon &polygon2, int i, int j) {
        Polygon mergedPolygon = outerface(polygon1, polygon2, i, j);

        std::cout << "Outer face: ";
        polygonal::printPolygon(mergedPolygon);

        auto minRectangle = minAreaRectangle(mergedPolygon);
        double minRectangleArea = minRectangle.first * minRectangle.second;
        if (minRectangleArea < minArea)
        {
            minArea = minRectangleArea;
            mergedItem = Item(mergedPolygon.size(), mergedPolygon);
        }
    };

    Polygon polygon1 = item1.vertices;
    Polygon polygon2 = polygonal::changeOrientation(item2.vertices);
    int n = polygon1.size();
    int m = polygon2.size();
    assert(n >= 3 and m >= 3);
    polygonal::printPolygon(polygon1);
    // for each edges of polygon1
    for (int i = 0; i < n; i++)
    {
        // edge_i (p1, p2)
        Point p1 = polygon1[i];
        Point p2 = polygon1[(i + 1) % n];
        p1.print();
        p2.print();
        std::cout << "A ::::::::\n";
        // for each edges of polygon2
        for (int j = 0; j < m; j++)
        {
            polygon2[j].print();
            polygon2[(j + 1) % m].print();
            std::cout << "B :::\n";

            Polygon tmpPolygon2 = polygonal::translateToNewOrign(p1, polygon2);
            tmpPolygon2 = polygonal::translateToNewOrign((tmpPolygon2[j] - p1) * -1, tmpPolygon2);

            std::cout << "After translation: ";
            polygonal::printPolygon(tmpPolygon2);

            assert(geo::dcmp(tmpPolygon2[j].x - p1.x, 1e-4) == 0);
            assert(geo::dcmp(tmpPolygon2[j].y - p1.y, 1e-4) == 0);

            // align the (p1, p3) edge with (p1, p2)
            Point p3 = tmpPolygon2[(j + 1) % m];
            double angleCCW1 = vec::getAngleInRad(p2 - p1, Point(1e8, p1.y) - p1);
            double angleCCW2 = vec::getAngleInRad(p3 - p1, Point(1e8, p1.y) - p1);
            double rotationAngle = angleCCW1 - angleCCW2;

            tmpPolygon2 = polygonal::rotateBy(rotationAngle, Point(0, 0), tmpPolygon2);

            std::cout << "After rotation by: " << geo::RAD2DEG(rotationAngle) << " degree: ";
            polygonal::printPolygon(tmpPolygon2);

            // check for intersection
            bool isIntersecting = polygonal::polygonIntersectPolygon(polygon1, tmpPolygon2);
            if (!isIntersecting)
            {
                checkForOptimal(polygon1, tmpPolygon2, i, j);
            }
            p3 = tmpPolygon2[(j + 1) % m];
            if (geo::dcmp(vec::lenSq(p2 - p2) - vec::lenSq(p3 - p1)) != 1)
            {
                // (p1, p3) >= (p1, p2)
                continue;
            }
            tmpPolygon2 = polygonal::translateToNewOrign(p2, tmpPolygon2);
            tmpPolygon2 = polygonal::translateToNewOrign((tmpPolygon2[(j + 1) % m] - p2) * -1, tmpPolygon2);

            isIntersecting = polygonal::polygonIntersectPolygon(polygon1, tmpPolygon2);
            if (!isIntersecting)
            {
                checkForOptimal(polygon1, tmpPolygon2, i, j);
            }
        }
    }
    return mergedItem;
}

/**
 * merge items by trying different rotations
*/
ItemState approach1::mergeItem(ItemState &itemState1, ItemState &itemState2)
{
    double angle = 90;
    double minArea = INF, length = INF;

    Item item1, item2, mergedItem;

    if (itemState1.second >= itemState2.second)
    {
        item1 = itemState1.first;
        item2 = itemState2.first;
    }
    else
    {
        item1 = itemState2.first;
        item2 = itemState1.first;
    }

    for (int i = 0; i < (360 / (int)angle); i++)
    {
        Item tmpMergedItem = mergeHeuristic1(item1, item2);
        auto minRectangle = minAreaRectangle(tmpMergedItem.vertices);
        double tmpArea = minRectangle.first * minRectangle.second;
        if (tmpArea < minArea)
        {
            mergedItem = tmpMergedItem;
            minArea = tmpArea;
            length = minRectangle.first;
        }
        else if (tmpArea == minArea and length > minRectangle.first)
        {
            mergedItem = tmpMergedItem;
            length = minRectangle.first;
        }
        item1 = item1.rotate(geo::DEG2RAD(angle), Point(0, 0));
    }

    return {mergedItem, minArea};
}

/**
 * divide and conquer approach
*/
ItemState approach1::split(std::vector<Item> &items, int left, int right)
{
    if (left == right)
    {
        auto minRectangle = minAreaRectangle(items[left].vertices);
        return {items[left], minRectangle.first * minRectangle.second};
    }
    int mid = left + (right - left) / 2;
    ItemState resultLeft = split(items, left, mid);
    ItemState resultRight = split(items, mid + 1, right);

    return mergeItem(resultLeft, resultRight);
}

/**
 * returns soltuion for different items order
*/
void approach1::solution(std::vector<Item> &items)
{
    ItemState result = split(items, 0, items.size() - 1);
}