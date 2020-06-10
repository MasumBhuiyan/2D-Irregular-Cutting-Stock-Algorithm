#include "Stock.hpp"

/** constructors */
Stock::Stock() { Stock::Matrix(); }
Stock::Stock(int row, int col) { Stock::Matrix(row, col); }

/** method */
/**
 *  is the Item(Matrix) placeable or not in the point
*/
bool Stock::isPlaceable(Point point, Matrix &matrix) 
{ 
	return this.isInstersecation(point, matrix); 
}

/**
 * insert the Item(Matrix) in Point
*/
void Stock::insertItem(Point point, Matrix &matrix) 
{
	auto row_col = matrix.getDimension();
    int _row = row_col.first;
    int _col = row_col.second;

    for(int i = 0; i < _row; i += 1) 
    {
        for(int j = 0; j < _col; j += 1)
        {
            assert( i + point.x >= 0 && i + point.x < row); 
            assert( j + point.y >= 0 && j + point.y < col); 
            mat[ i + point.x ][ j + point.y ] == matrix.mat[ i ][ j ];
        }
    }
}