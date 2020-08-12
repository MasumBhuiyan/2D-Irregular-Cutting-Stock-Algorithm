#include <bin_packing.hpp>

void testDataset(std::string datasetName)
{
    double width;
    std::vector<Polygon> inputPolygons;
    std::tie(inputPolygons, width) = bin_packing::readDataset(datasetName);
    bin_packing::binPacking(inputPolygons, width, "../io/results", datasetName, 5.0);
}

int main(int argc, char const *argv[])
{
    string outputLocation = "../io/results";
    mkdir(outputLocation.c_str(), 0777);

    // testDataset("sample");
    // testDataset("albano");
    // testDataset("blaz");
    // testDataset("dagli");
     testDataset("dighe1");
    // testDataset("jakobs1");
    // testDataset("jakobs2");
    // testDataset("marques");
    // testDataset("poly1a");
    // testDataset("shirts");
    // testDataset("trousers");
    return 0;
}
