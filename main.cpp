#include <bin_packing.hpp>


void testDataset(std::string datasetName)
{
    std::string outputDirectoryName = "../io/results/" + datasetName;
    mkdir(outputDirectoryName.c_str(), 0777);

    double width;
    std::vector<Polygon> inputPolygons;
    std::tie(inputPolygons, width) = bin_packing::readDataset(datasetName);

    

    int i = 0;
    for(auto p : inputPolygons)
    {
        if( boost_geo::is_valid(p) == false )
        {
            std::cout << datasetName << ": polygon " << i << " is invalid\n"; 
        }
        i += 1;
    }
   bin_packing::binPacking(inputPolygons, width, "../tests/results/blf/", datasetName, 5.0);
}

int main(int argc, char const *argv[])
{
    // testDataset("sample");
    // testDataset("albano");
    // testDataset("blaz");
    // testDataset("dagli");
    // testDataset("dighe1");
     testDataset("jakobs1");
    // testDataset("jakobs2");
    // testDataset("marques");
    // testDataset("poly1a");
    // testDataset("shirts"); // problem
    // testDataset("trousers");
    return 0;
}