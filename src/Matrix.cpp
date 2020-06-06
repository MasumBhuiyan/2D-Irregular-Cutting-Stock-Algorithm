#include "Matrix.hpp"

/** constructors */
Matrix::Matrix() {}
Matrix::Matrix(int r, int c) : row(r), col(c)
{
    mat.resize(row, std::vector<bool>(col, false));
}

/** methods */
/**
 * returns {row, col}
*/
std::pair<int, int> Matrix::getDimention() { return {row, col}; }

/**
 * checks the provided matrix is intersecting or not 
 * if it is placed in Point
*/
bool Matrix::isIntersecting(Point point, Matrix &matrix) { return false; }