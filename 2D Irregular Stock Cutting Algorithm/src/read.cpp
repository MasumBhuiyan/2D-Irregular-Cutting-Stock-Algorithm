#include "read.h"


vector<polygon> read() {
    int n; cin >> n;
    vector<polygon> polygons(n);
    for(auto& poly : polygons) poly.read();

    for(int i = 0; i < n; i += 1) {
        cout << "Polygon[" << i << "]\n";
        polygons[ i ].print(); cout << endl;
    }

    return polygons;
}
