#include "Matrix.hpp"

/** constructors */
Matrix::Matrix() {}
Matrix::Matrix(int r, int c) : row(r), col(c) { mat.resize(row, vector<bool>(col, false)); }

/** methods */
pair<int, int> Matrix::getDimention() { return {row, col}; }