#ifndef STOCK_HPP
#define STOCK_HPP

#include "Matrix.hpp"

class Stock : public Matrix
{
private:
    /* data */
public:
    Stock();
    Stock(int, int);
    std::pair<int,int> getDimension();

    // methods
    bool isPlaceable(Point, Matrix &); // is the Item placeable or not in the point
    void insertItem(Point, Matrix &);  // insert the Item in Point
};

#endif // STOCK_HPP