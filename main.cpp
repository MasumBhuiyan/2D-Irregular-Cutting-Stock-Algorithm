#include <bin_packing.hpp>

void testDataset(std::string datasetName, bool flag = true)
{
    double width;
    std::vector<Polygon> inputPolygons;
    std::tie(inputPolygons, width) = bin_packing::readDataset(datasetName);
    bin_packing::binPacking(inputPolygons, width, "../io/results", datasetName, 5.0);
}

int main(int argc, char const *argv[])
{
    // string outputLocation = "../io/results";
    // mkdir(outputLocation.c_str(), 0777);

    testDataset("sample");  // 100.00% // no cluster: 50.00% //
    testDataset("dighe1"); //  67.42% // no cluster: 66.71% //
    // testDataset("albano");  //  67.89% // no cluster: 80.26% //
    // testDataset("blaz");    //  74.06% // no cluster: 69.67% //
    // testDataset("dagli");   //  70.13% // no cluster: 73.67% //
    // testDataset("jakobs1"); //  68.92% // no cluster: 72.37% //
    // testDataset("jakobs2"); //  57.69% // no cluster: 69.01% //
    // testDataset("trousers");//  63.87% // no cluster: 82.50% //
    // testDataset("poly1a");  //  55.92% // no cluster: 57.86% //
    return 0;
}
