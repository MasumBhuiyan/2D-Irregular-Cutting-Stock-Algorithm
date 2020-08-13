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

    // testDataset("sample");  // 100.00%
    // testDataset("albano");  //  67.89%
    // testDataset("blaz");    //  74.06%
    // testDataset("dagli");   //  70.13% 
    // testDataset("dighe1");  //  67.42%
    // testDataset("jakobs1"); //  68.92%
    // testDataset("jakobs2"); //  57.69%
    // testDataset("trousers");//  63.87%
    // testDataset("marques");  // unable complete outer loop 
    // testDataset("poly1a"); // all pair nfp not computed
    // testDataset("shirts"); // all pair nfp not computed
    return 0;
}
