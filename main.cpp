#include <bin_packing.hpp>

void testDataset(std::string datasetName)
{
    std::string outputDirectoryName = "../io/results/" + datasetName;
    mkdir(outputDirectoryName.c_str(), 0777);

    double width;
    std::vector<Polygon> inputPolygons;
    std::tie(inputPolygons, width) = bin_packing::readDataset(datasetName);
}

int main(int argc, char const *argv[])
{
    testDataset("sample");
    testDataset("albano");
    testDataset("blaz");
    testDataset("dagli");
    testDataset("dighe1");
    testDataset("jakobs1");
    testDataset("jakobs2");
    testDataset("marques");
    testDataset("poly1a");
    testDataset("shirts");
    testDataset("trousers");
    return 0;
}