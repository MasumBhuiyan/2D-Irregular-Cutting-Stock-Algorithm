#include <bin_packing.hpp>

void get_cluster_values_test(std::string datasetName)
{
    vector<Polygon> inputPolygons;
    double width;
    std::tie(inputPolygons, width) = bin_packing::readDataset(datasetName);
    std::cout << inputPolygons.size() << " " << width << "\n";
    vector<vector<vector<vector<double>>>> clusterValues = cluster_util::getClusterValues(inputPolygons, datasetName);

    for(int i = 0; i < clusterValues.size(); i += 1) 
    {
        for(int j = 0; j < clusterValues.size(); j += 1)
        {
            for(int k = 0; k < 4; k += 1) 
            {
                for(int l = 0; l < 4; l += 1)                
                {
                    std::cout << clusterValues[i][j][k][l] << " ";
                }
            }
            std::cout << "\n";
        }  
    }
}
int main()
{
    get_cluster_values_test("jakobs1");
    return 0;
}