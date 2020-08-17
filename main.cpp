#include <bin_packing.hpp>

void testDataset(std::string datasetName, bool flag = true)
{
    double width;
    std::vector<Polygon> inputPolygons;
    std::tie(inputPolygons, width) = bin_packing::readDataset(datasetName);
    if (flag)
    {
        // bin_packing::binPacking(inputPolygons, width, "../io/results", datasetName, 5.0);
    }
    else
    {
        bin_packing::cuckooPacking(datasetName, "../io/results", width, 320);
    }
}

int main(int argc, char const *argv[])
{
    string outputLocation = "../io/results";
    mkdir(outputLocation.c_str(), 0777);

    // testDataset("sample", argc == 0);  
    // testDataset("albano", argc == 0);  
    // testDataset("blaz", argc == 0);    
    // testDataset("dagli", argc == 0);   
    testDataset("dighe1", argc == 0); //  
    // testDataset("jakobs1", argc == 0); 
    // testDataset("jakobs2", argc == 0); 
    // testDataset("trousers", argc == 0);
    // testDataset("poly1a", argc == 0);  
    // testDataset("shirts", argc == 0);  
    return 0;
}
