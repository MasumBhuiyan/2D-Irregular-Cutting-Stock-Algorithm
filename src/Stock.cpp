#include "Stock.hpp"

/** constructors */
Stock::Stock() { Stock::Matrix(); }
Stock::Stock(int _row, int _col) 
{	
	row = _row;
	col = _col;
	mat.resize(row, std::vector<bool>(col, false));
}
std::pair<int,int> Stock::getDimension() { return {row, col}; }


/** method */
/**
 *  is the Item(Matrix) placeable or not in the point
*/
bool Stock::isPlaceable(Point point, Matrix &matrix) 
{ 
	std::pair<int,int> row_col = matrix.getDimension();
    int _row = row_col.first;
    int _col = row_col.second;

    for(int i = point.x; i < std::min(row, (int)point.x + _row); i += 1) 
    {
        for(int j = point.y; j < std::min(col, (int)point.y + _col); j += 1) 
        {
            if( matrix.mat[ i - point.x ][ j - point.y ] == 1 and mat[ i ][ j ] == 1 )
            {
                return  false;
            }
        }
    }
    return true;
}

/**
 * insert the Item(Matrix) in Point
*/
void Stock::insertItem(Point point, Matrix &matrix) 
{
	std::pair<int,int> row_col = matrix.getDimension();
    int _row = row_col.first;
    int _col = row_col.second;

    for(int i = point.x; i < std::min(row, (int)point.x + _row); i += 1) 
    {
        for(int j = point.y; j < std::min(col, (int)point.y + _col); j += 1) 
        {
            if( matrix.mat[ i - point.x ][ j - point.y ] == true )
            {
            	mat[ i ][ j ] = true;
            }
        }
    }
}