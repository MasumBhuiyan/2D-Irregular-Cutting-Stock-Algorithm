#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>

using namespace std;

class Matrix
{
private:
    /* data */
    int row, col;
    vector<vector<bool>> mat;

public:
    Matrix(/* args */);
    ~Matrix();

    // getter & setter
    int getRow();
    void setRow(int);

    int getCol();
    void setCol(int);

    vector<vector<bool>> getMat();
    void setMat(vector<vector<bool>>);
};

#endif // MATRIX_HPP