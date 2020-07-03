#include <_2D_irregular_bin_packing.hpp>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>

using namespace chrono;

void test_with_dataset(string datasetFileName)
{
    std::cout << "Dataset: " << datasetFileName << std::endl;

    auto start = high_resolution_clock::now();

    double width;
    std::vector<int> quantities;
    std::vector<Polygon> polygons;
    dnc_approach::readIO(datasetFileName, polygons, quantities, width);

    double area = 0.0;
    for (auto poly : polygons)
    {
        area += dnc_approach::areaPoly(poly);
    }

    std::vector<Polygon> stocks;
    Polygon stock;

    tie(stocks, stock) = dnc_approach::split(polygons, 0, polygons.size() - 1);
    dnc_approach::processVisualize(stocks);
    //  processVisualize({stock});

    cout << stocks.size() << endl;

    double area1 = dnc_approach::areaPoly(stock), area2 = dnc_approach::minBoundingRectangle(stock);
    cout << "total area: " << area << '\n';
    cout << "area1: " << area1 << '\n';
    cout << "area2: " << area2 << '\n';
    std::cout << "Packing Density:: " << (area1 / area2) * 100 << " %\n";

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() / 1000000.0 << " seconds" << std::endl;
}

int main()
{
    test_with_dataset("../../tests/dataset/sample.txt");
    test_with_dataset("../../tests/dataset/albano.txt");
    test_with_dataset("../../tests/dataset/blaz.txt");
    test_with_dataset("../../tests/dataset/dagli.txt");
    test_with_dataset("../../tests/dataset/jakobs1.txt");
}
