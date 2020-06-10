#include "Matrix.hpp"
#include <assert.h>

/** constructors */
Matrix::Matrix() {}
Matrix::Matrix(int r, int c) : row(r), col(c)
{
    mat.resize(row, std::vector<bool>(col, false));
}

/** methods */
/**
 * Prints matrix on console
*/
void Matrix::print()
{
    std::cout << "dimension: " << row << " x " << col << std::endl;
    for (int j = 0; j < col; j++)
    {
        for (int i = 0; i < row; i++)
        {
            if (mat[i][j] == 1)
            {
                std::cout << "# ";
            }
            else
            {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * returns {row, col}
*/
std::pair<int, int> Matrix::getDimension() { return {row, col}; }

/**
 * checks the provided matrix is intersecting or not 
 * if it is placed in Point
 * 1 means intersecting, 0 means not
*/
bool Matrix::isIntersecting(Point point, Matrix &matrix) 
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
            if( matrix.mat[ i ][ j ] == 1 and  mat[ i + point.x ][ j + point.y ] == 1)
            {
                return true;
            }
        }     
    }
    return false;
}

void Matrix::addRow()
{
    std::vector<bool> new_row(col, 0);
    mat.emplace_back(new_row);
}