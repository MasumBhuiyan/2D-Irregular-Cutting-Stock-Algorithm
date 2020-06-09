#include "rasterization.hpp"
#include "Item.hpp"
#include "Matrix.hpp"

void rasterize_test1()
{
	Item item(10,
			  {{4 * 2, 4 * 6},
			   {4 * 7, 4 * 1},
			   {4 * 6, 4 * 4},
			   {4 * 10, 4 * 4},
			   {4 * 7, 4 * 3},
			   {4 * 11, 4 * 2},
			   {4 * 11, 4 * 8},
			   {4 * 9, 4 * 5},
			   {4 * 6, 4 * 10},
			   {4 * 6, 4 * 6}});
	Matrix matrix = item.rasterize();

	pair<int, int> row_col = matrix.getDimension();
	int row = row_col.first;
	int col = row_col.second;

	matrix.print();
}

void rasterize_test2()
{
	Polygon polygon({{39.8, 59.1},
					 {36.1, 58.1},
					 {32.9, 57.5},
					 {22.9, 57},
					 {22, 53.8},
					 {22, 53.8},
					 {22, 52},
					 {22.7, 49.2},
					 {31.6, 48.1},
					 {32, 36.9},
					 {32, 36.9},
					 {31.6, 27.3},
					 {30.9, 20.9},
					 {35.4, 19.1},
					 {40.7, 18.2},
					 {45.5, 18.2},
					 {50.5, 18.2},
					 {50.5, 18.2},
					 {57.8, 19.6},
					 {59.2, 20},
					 {59.2, 20},
					 {58.3, 32.1},
					 {58.3, 32.1},
					 {58.3, 36.2},
					 {57.8, 48.1},
					 {57.8, 48.1},
					 {62.4, 49.5},
					 {66.3, 49.5},
					 {66.3, 49.5},
					 {67.2, 53.6},
					 {67.2, 53.6},
					 {66.7, 55.4},
					 {66.5, 57},
					 {66.5, 57},
					 {63.8, 57},
					 {59.6, 57.5},
					 {54.4, 58.1},
					 {49.1, 59.3},
					 {45.3, 58.6},
					 {45.3, 58.6},
					 {43.4, 58.6},
					 {42.1, 58.4}});
	std::reverse(polygon.begin(), polygon.end());
	Item item(polygon.size(), polygon);

	Matrix matrix = item.rasterize();
	matrix.print();
}

void rasterize_test3()
{
	Item item(16,
			  {{0, 0},
			   {15, 0},
			   {15, 12},
			   {14, 12},
			   {13, 13},
			   {13, 15},
			   {15, 18},
			   {10, 20},
			   {9, 19},
			   {6, 19},
			   {5, 20},
			   {0, 18},
			   {2, 15},
			   {2, 13},
			   {1, 12},
			   {0, 12}});
	Matrix matrix = item.rasterize();

	pair<int, int> row_col = matrix.getDimension();
	int row = row_col.first;
	int col = row_col.second;

	matrix.print();
}

void rasterize_test4()
{
	Polygon polygon({{43.7, 73.7},
					 {9.7, 51.4},
					 {11.9, -17.9},
					 {21.9, 14.8},
					 {21.9, 14.8},
					 {41.1, -11.3},
					 {80.8, -4.8},
					 {100.4, 24.4},
					 {100.9, 51.9},
					 {92.6, 73.7},
					 {93.9, 27.9},
					 {79.5, 83.3},
					 {79.5, 83.3},
					 {79.1, 19.6},
					 {52, -0.9},
					 {52, -0.4},
					 {67.3, 27.9},
					 {70.3, 39.2},
					 {46.8, 29.2},
					 {31.1, 38.4},
					 {22.4, 43.6},
					 {43.7, 44},
					 {61.2, 47.5},
					 {73.4, 58.4},
					 {73, 69.3},
					 {76.9, 78.9},
					 {49.8, 88.5},
					 {14.1, 85.9},
					 {6.2, 70.2},
					 {18.4, 81.1},
					 {9.3, 61.9},
					 {25.9, 79.4},
					 {24.1, 68.5}});
	std::reverse(polygon.begin(), polygon.end());
	Item item(polygon.size(), polygon);

	Matrix matrix = item.rasterize();
	matrix.print();
}

int main()
{
	rasterize_test1();
	rasterize_test2();
	rasterize_test3();
	rasterize_test4();
}