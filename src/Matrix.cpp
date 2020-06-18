#include "Matrix.hpp"

/** constructors */
Matrix::Matrix() {}
Matrix::Matrix(int r, int c) : row(r), col(c)
{
    mat.resize(row, std::vector<int>(col, 0));
}

/** methods */
/**
 * Prints matrix on console
*/
void Matrix::print()
{
    std::cout << "dimension: " << row << " x " << col << std::endl;
    for (int i = 0; i < row; i += 1)
    {
        for (int j = 0; j < col; j += 1)
        {
            if( mat[ i ][ j ] == 0) {
                if( i == 0 || i == mat.size() - 1 ) { 
                    std::cout << "-";
                } else if( j == 0 || j == mat[ 0 ].size() - 1 ) {
                    std::cout << "|";
                }
                else std::cout << " ";
            }
            else std::cout << mat[ i ][ j ];
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
*/
bool Matrix::isIntersecting(Point point, Matrix &matrix) 
{
    std::pair<int,int> row_col = matrix.getDimension();
    int _row = row_col.first;
    int _col = row_col.second;

    for(int i = point.x; i < std::min(row, (int)point.x + _row); i += 1) 
    {
        for(int j = point.y; j < std::min(col, (int)point.y + _col); j += 1) 
        {
            if( matrix.mat[ i - point.x ][ j - point.y ] and mat[ i ][ j ] )
            {
                return  true;
            }
        }
    }
    return false;
}