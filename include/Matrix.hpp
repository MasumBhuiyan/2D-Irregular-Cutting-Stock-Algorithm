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

    Matrix(/* args */);
    ~Matrix();

    // methods
    vector<vector<bool>> getMat();
    void setMat(vector<vector<bool>>);
};

#endif // MATRIX_HPP