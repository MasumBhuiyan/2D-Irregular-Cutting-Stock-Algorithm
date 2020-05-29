#include <iostream>
#include <geometry.hpp>

using namespace std;

int main(int argc, char const *argv[]) {

	cout << "testing polygon structure...\n";
    polygon p, p0;
    p.read();
    p0.read();
    p.print();
    p.area();
    p = p.rotate(180);
    p.print();

    cout << "seems okay!!!\n";

    cout << "testing minRectangleEnclosure() function...\n";
    int x, y; tie(x, y) = minRectangleEnclosure(p);
    cout << "Given: " << x << " " << y << "\n";
    cout << "Expected: 7 4\nokay\n";

    vector<polygon> polygons;
    polygons.emplace_back(p);
    polygons.emplace_back(p0);

    polygons = sort(polygons);
    polygons[ 0 ].print();
    polygons[ 1 ].print();


    tie(x, y) = orient(p);
    
    return 0;
}
/*
12
0 2
1 1
2 0
3 0
4 0
5 0
6 1
7 2
5 3
4 4
3 4
2 3

22
1 0
2 0
3 0
4 0
5 0
6 0
6 1
6 2
6 3
6 4
6 5
7 6
5 7
4 6
3 6
2 7
0 6
1 5
1 4
1 3
1 2
1 1
*/