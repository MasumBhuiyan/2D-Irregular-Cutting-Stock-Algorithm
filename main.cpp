#include <_2D_irregular_bin_packing.hpp>

std::vector<PolygonInput> readDataset(std::string filename, std::vector<int> &quantity, double &width)
{
    std::vector<PolygonInput> items;
    std::ifstream file(filename);
    if (file.fail())
    {
        std::cerr << "file does not exists\n";
        exit(-1);
    }
    else
    {
        int n;
        file >> width;
        file >> n;
        quantity.resize(n);
        for (int i = 0; i < n; i += 1)
        {
            PolygonInput item;
            file >> quantity[i];
            // reading polygon counter clockwise as given in the dataset
            int m;
            file >> m;
            while (m--)
            {
                double _x, _y;
                file >> _x >> _y;
                item.push_back({_x, _y});
            }
            for (int k = 0; k < quantity[i]; k += 1)
            {
                items.push_back(item);
            }
        }
    }
    file.close();
    return items;
}

void runDataset(std::string datasetName)
{
    double width;
    std::vector<int> quantity;
    std::vector<PolygonInput> items = readDataset(datasetName, quantity, width);
    dnc_approach::solution(items);
}

int main(int argc, char const *argv[])
{
    runDataset("../tests/dataset/sample.txt");
    runDataset("../tests/dataset/albano.txt");
    runDataset("../tests/dataset/blaz.txt");
    runDataset("../tests/dataset/dagli.txt");
    runDataset("../tests/dataset/jakobs1.txt");
    return 0;
}