#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>

using namespace std;

class Matrix
{
private:
public:
    int row, col;
    vector<vector<bool>> mat;

    Matrix();
    Matrix(int, int);
    ~Matrix();

    // methods
    pair<int, int> getDimention();
};

#endif // MATRIX_HPP