#include "rasterization.hpp"
#include "Matrix.hpp"
#include "custom_assertion.hpp"

void regionfill_test1()
{
	Matrix matrix(9, 10);
	matrix.mat[1][2] = 1; 
	matrix.mat[1][4] = 1; 
	matrix.mat[1][5] = 1; 
	matrix.mat[1][6] = 1; 
	matrix.mat[1][7] = 1; 
	matrix.mat[2][1] = 1; 
	matrix.mat[2][3] = 1; 
	matrix.mat[2][8] = 1; 
	matrix.mat[3][0] = 1; 
	matrix.mat[3][9] = 1; 
	matrix.mat[4][0] = 1; 
	matrix.mat[4][9] = 1; 
	matrix.mat[5][0] = 1; 
	matrix.mat[5][4] = 1; 
	matrix.mat[5][5] = 1; 
	matrix.mat[5][9] = 1; 
	matrix.mat[6][0] = 1; 
	matrix.mat[6][3] = 1; 
	matrix.mat[6][6] = 1; 
	matrix.mat[6][9] = 1; 
	matrix.mat[7][0] = 1; 
	matrix.mat[7][1] = 1; 
	matrix.mat[7][2] = 1; 
	matrix.mat[7][6] = 1; 
	matrix.mat[7][9] = 1; 
	matrix.mat[8][7] = 1; 
	matrix.mat[8][8] = 1; 

	raster::regionfill(matrix, 3, 4);

	std::cout << "Printing matrix to see region fill works or not?\n"; 
	for(int i = 0; i < 9; i += 1) 
	{
		for(int j = 0; j < 10; j += 1)
		{
			if( matrix.mat[ i ][ j ] ) 
			{
				cout << "# ";
			}
			else
			{
				cout << ". ";
			}
		}
		cout << "\n";
	}
	cout << "printing ends\n";
}
int main() 
{
	regionfill_test1();
}