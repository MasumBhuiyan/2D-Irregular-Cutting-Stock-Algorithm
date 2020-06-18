#include "algorithm.hpp"
#include <fstream>

void split_test1()
{
	std::ifstream file("../../tests/src/algorithm/split_test1_input.txt");
	if (file.fail())
    {
        std::cerr << "File does not exists" << "\n";
        exit(-1);
    }
	int n; file >> n;
	std::vector<Matrix> Items(n);
 

	for(int i = 0; i < n; i += 1) 
	{
		int r, c; file >> r >> c;
		//std::cout << r << " " << c << "\n";
		int m; file >> m;

		Items[ i ].row = r;
		Items[ i ].col = c;
		Items[ i ].mat.resize(r, std::vector<int>(c, 0));

		//std::cout << (int)Items[ i ].mat.size() << "\n";
		while( m-- ) {
			int x, y; file >> x >> y;
			Items[ i ].mat[ x ][ y ] = i + 1;
		}
	}

	// print items
	for(int i = 0; i < n; i += 1) {
		std::cout << "Item " << i << "::\n";
		Items[ i ].print();
	}

	Matrix stock = split(Items, 0, n - 1);
	//Matrix stock = mergeItems(Items[ 0 ], Items[ 1 ]);

	std::cout << "Stock::\n";
	stock.print();

	double packingDensity = 0;
	for(int i = 0; i < stock.row; i += 1) {
		for(int j = 0; j < stock.col; j += 1) {
			packingDensity += (stock.mat[ i ][ j ] == 0 ? 0 : 1);
		}
	}
	packingDensity /= ((stock.row - 1) * (stock.col - 1));
	std::cout << "Packing Density: " << packingDensity * 100 << "%" << "\n";
}
int main()
{
	split_test1();
	return 0;
}