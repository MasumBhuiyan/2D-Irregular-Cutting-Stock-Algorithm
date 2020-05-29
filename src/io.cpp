#include <iostream>

#include "io.hpp"

vector<polygon> io::readInputFile(string filename, double &width)
{
    cout << "Opening file: " << filename << " ....." << endl;
    ifstream file(filename);
    if (file.fail())
    {
        cerr << "File does not exists" << endl;
        exit(-1);
    }
    cout << "Reading file..." << endl;

    /* width of the bean */
    file >> width;

    /* number of pieces/polygons */
    int numberOfPolygons;
    file >> numberOfPolygons;
    vector<polygon> polygons(numberOfPolygons);

    /* input all the polygons */
    for (auto &p : polygons)
    {
        file >> p.n;
        p.v.resize(p.n);
        for (auto &vi : p.v)
        {
            file >> vi.x >> vi.y;
        }
    }
    cout << "Reading complete" << endl;
    file.close();
    return polygons;
}