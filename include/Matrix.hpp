#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>

#include "Point.hpp"

class Matrix
{
private:
public:
    int row, col;
    std::vector<std::vector<bool>> mat;

    Matrix();
    Matrix(int, int);

    // methods
    void print();                         // prints the matrix on console
    std::pair<int, int> getDimension();   // returns {row, col}
    bool isIntersecting(Point, Matrix &); // checks the provided matrix is intersecting or not if it is placed in Point
    void addRow();                        // add a new row
    void insertItem(Point, Matrix &);     // insert the Item in Point
};

#endif // MATRIX_HPP