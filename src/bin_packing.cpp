#include <bin_packing.hpp>

std::tuple<std::vector<Polygon>, double> bin_packing::readDataset(std::string datasetName)
{
	double width;
	std::vector<Polygon> polygons;
	std::ifstream file("../io/datasets/" + datasetName + ".txt");
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
		Polygon polygon;
		for (int i = 0; i < n; i += 1)
		{
			int quantity;
			file >> quantity;
			int numberOfVirtices;
			file >> numberOfVirtices;
			for (int j = 0; j < numberOfVirtices; j++)
			{
				double _x, _y;
				file >> _x >> _y;
				polygon.outer().push_back(Point(_x, _y));
			}
			polygon.outer().push_back(polygon.outer()[0]);
			reverse(polygon.outer().begin(), polygon.outer().end());
            while (quantity--)
			{
				polygons.push_back(polygon);
			}
		}
	}
	file.close();
	return {polygons, width};
}