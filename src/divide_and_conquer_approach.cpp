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
std::tuple<Point,Point, int> approach1::findLargestEdge(Item &item)
{
    int noOfVertices = item.vertices.size();
    double d = 0;
    Point p, q;
    int k;
    for(int i = 0; i < noOfVertices; i += 1) 
    {
        Point _p = item.vertices[ i ];
        Point _q = item.vertices[ (i + 1) % noOfVertices ];
        double _d = (_p.x - _q.x) * (_p.x - _q.x) + (_p.y - _q.y) * (_p.y - _q.y);
        if( _d > d )
        {
            d = _d;
            p = _p;
            q = _q;
            k = i;
        }
    }
    return {p, q, k};
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
    for(auto &point : item.vertices)
    {
        point = point.rotate(r, angle * isLeft); 
        point = point - r;
        point = point + p;
    }
    return item;
}

/**
 * returns the outerface of the Item a, and b after placing 
 * Item b's point r at Item a's point p
*/
Item approach1::outerface(Item &item1, Item &item2, int k1, int k2, int x, int y)
{
    std::vector<Point> points;   
    int lena = item1.vertices.size();
    int lenb = item2.vertices.size();

    if( x == 0 && y == 0 )
    {
        for(int i = 0; i < k1; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
        for(int i = k2; ; i = (((i - 1) % lenb) + lenb)%lenb)
        {
            points.push_back(item2.vertices[ i ]);
            if( i == ((k2 + 1) % lenb) ) break;
        }
        for(int i = k1 + 1; i < lena; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
    }
    else if( x == 0 && y == 1 )
    {
        for(int i = 0; i < k1; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
        for(int i = (k2 + 1) % lenb; ; i = (i + 1) % lenb)
        {
            points.push_back(item2.vertices[ i ]);
            if( i == k2 ) break;
        }
        for(int i = k1 + 1; i < lena; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
    }
    else if( x == 1 && y == 0 )
    {
        for(int i = 0; i <= k1; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
        for(int i = (k2 + 1) % lenb; ; i = (i + 1) % lenb)
        {
            points.push_back(item2.vertices[ i ]);
            if( i == k2 ) break;
        }
        for(int i = k1 + 2; i < lena; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
    }
    else
    {
        for(int i = 0; i <= k1; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
        for(int i = k2; ; i = (((i - 1) % lenb) + lenb)%lenb)
        {
            points.push_back(item2.vertices[ i ]);
            if( i == ((k2 + 1) % lenb) ) break;
        }
        for(int i = k1 + 2; i < lena; i += 1)
        {
            points.push_back(item1.vertices[ i ]);
        }
    }
    Item face(points.size(), points);
    return face;
}

/**
 * returns the min bounding rectangle axis parallel
*/
std::pair<double, double> approach1::minAreaRectangle(Item &item)
{
    double min_x = INF, max_x = -INF;
    double min_y = INF, max_y = -INF;

    for (Point vertex : item.vertices)
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
    
    if( (x2 - x1) < geo::EPS )
    {
        for (auto &point : item.vertices)
        {
            point.x = 2 * x1 - point.x; 
        }
    }
    else if( (y2 - y1) < geo::EPS )
    {
        for (auto &point : item.vertices)
        {
            point.y = 2 * y1 - point.y; 
        }
    } 
    else
    {
       double m = (y2 - y1) / (x2 - x1);
       double b = y1 - m * x1;
        
        for (auto &point : item.vertices)
        {
            double x = point.x;
            double y = point.y; 
            point.x = ((1 - m * m) * x + 2 * m * y - 2 * m * b) / (m * m + 1);
            point.y = ((m * m - 1) * y + 2 * m * x + 2 * b) / (m * m + 1);
        }
    }
    return item;
}
/**
 * merges two item by proposed heuristic1
*/
Item approach1::mergeHeuristic1(Item &item1, Item &item2)
{
    Item mergedItem;

    Point p, q, r, s;
    int k1, k2; 

    tie(p, q, k1) = findLargestEdge(item1);
    tie(r, s, k2) = findLargestEdge(item2);
    
    Item item2PR = placement(item2, p, q, r, s);
    Item item2PS = placement(item2, p, q, s, r);
    Item item2QR = placement(item2, q, p, r, s);
    Item item2QS = placement(item2, q, p, s, r);

    if( polygonal::polygonIntersectPolygon(item1.vertices, item2PR.vertices) == true )
    {
        int len = item2PR.vertices.size();
        Point a = item2PR.vertices[ k2 ];
        Point b = item2PR.vertices[ (k2 + 1) % len ];
        item2PR = reflectAcrossLine(item2PR, a, b);
    }
    if( polygonal::polygonIntersectPolygon(item1.vertices, item2PS.vertices) == true )
    {
        int len = item2PS.vertices.size();
        Point a = item2PS.vertices[ k2 ];
        Point b = item2PS.vertices[ (k2 + 1) % len ];
        item2PR = reflectAcrossLine(item2PS, a, b);
    }
    if( polygonal::polygonIntersectPolygon(item1.vertices, item2QR.vertices) == true )
    {
        int len = item2QR.vertices.size();
        Point a = item2QR.vertices[ k2 ];
        Point b = item2QR.vertices[ (k2 + 1) % len ];
        item2PR = reflectAcrossLine(item2QR, a, b);
    }
    if( polygonal::polygonIntersectPolygon(item1.vertices, item2QS.vertices) == true )
    {
        int len = item2QS.vertices.size();
        Point a = item2QS.vertices[ k2 ];
        Point b = item2QS.vertices[ (k2 + 1) % len ];
        item2PR = reflectAcrossLine(item2QS, a, b);
    }

    std::vector<Item> outerfaces({ 
        outerface(item1, item2PR, k1, k2, 0, 0), 
        outerface(item1, item2PS, k1, k2, 0, 1),
        outerface(item1, item2QR, k1, k2, 1, 0), 
        outerface(item1, item2QS, k1, k2, 1, 1)});
    
    std::vector<tuple<double,double,int>> areas;
    for(int i = 0; i < 4; i += 1) 
    {
        auto minRectangle = minAreaRectangle(outerfaces[ i ]);
        areas.push_back({minRectangle.first * minRectangle.second, minRectangle.first, i}); 
    }
    sort(areas.begin(), areas.end());

    int length, width, i;
    tie(length, width, i) = areas[ 0 ];
    mergedItem = outerfaces[ i ];
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
        auto minRectangle = minAreaRectangle(tmpMergedItem);
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
        auto minRectangle = minAreaRectangle(items[left]);
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