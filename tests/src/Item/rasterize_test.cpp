#include "rasterization.hpp"
#include "Item.hpp"
#include "Matrix.hpp"


void rasterize_test1()
{
	Item item(10, 
			{
			    {2,6},
			    {7,1},
			    {6,4},
			    {10,4},
			    {7,3},
			    {11,2},
			    {11,8},
			    {9,5},
			    {6,10},
			    {6,6}
			});
	Matrix matrix = item.rasterize();

	pair<int,int> row_col = matrix.getDimension();
	int row = row_col.first;
	int col = row_col.second;

	for(int i = 0; i < row; i += 1)
	{
		for(int j = 0; j < col; j += 1)
		{
			if( matrix.mat[ i ][ j ] ) 
			{
					cout << ".";
			}
			else
			{
				cout << "";
			}
		}
		cout << endl;
	}
}
void rasterize_test2()
{
	Item item(5, 
			{
			    {5,3},
			    {15,3},
			    {15,13},
			    {5,13},
			    {10,8}
			});
	Matrix matrix = item.rasterize();

	pair<int,int> row_col = matrix.getDimension();
	int row = row_col.first;
	int col = row_col.second;

	for(int i = 0; i < row; i += 1)
	{
		for(int j = 0; j < col; j += 1)
		{
			if( matrix.mat[ i ][ j ] ) 
			{
					cout << "*";
			}
			else
			{
				cout << "-";
			}
		}
		cout << endl;
	}
}
void rasterize_test3()
{
	Item item(16, 
			{
			    {0,0},
			    {15,0},
			    {15,12},
			    {14,12},
			    {13,13},
			    {13,15},
			    {15,18},
			    {10,20},
			    {9,19},
			    {6,19},
			    {5,20},
			    {0,18},
			    {2,15},
			    {2,13},
			    {1,12},
			    {0,12}
			});
	
	Matrix matrix = item.rasterize();

	pair<int,int> row_col = matrix.getDimension();
	int row = row_col.first;
	int col = row_col.second;

	for(int i = 0; i < row; i += 1)
	{
		for(int j = 0; j < col; j += 1)
		{
			if( matrix.mat[ i ][ j ] ) 
			{
					cout << "*";
			}
			else
			{
				cout << "-";
			}
		}
		cout << endl;
	}
}
int main()
{
	rasterize_test1();
	rasterize_test2();
	rasterize_test3();
}