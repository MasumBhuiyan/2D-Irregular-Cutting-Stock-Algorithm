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

    // testDataset("sample");  // 100%
    // testDataset("albano");  // topological dimension error comming from libnfporb library after calling blf
    // testDataset("blaz");    // unable to compute nfp outer loop ....when call blf
    // testDataset("dagli");   // unable to compute nfp outer loop ....when call blf
    // testDataset("dighe1");  // 71.41%
    // testDataset("jakobs1"); // 68.93%
    // testDataset("jakobs2"); // 57.69%
    // testDataset("marques"); // unable complete outer loop 
    // testDataset("poly1a"); // all pair nfp not computed
    // testDataset("shirts"); // all pair nfp not computed
    // testDataset("trousers"); // topological dimension error comming from libnfporb library after calling blf
    return 0;
}
