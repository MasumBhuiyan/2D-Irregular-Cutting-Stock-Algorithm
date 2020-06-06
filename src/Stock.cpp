#include "Stock.hpp"

/** constructors */
Stock::Stock() { Stock::Matrix(); }
Stock::Stock(int row, int col) { Stock::Matrix(row, col); }

/** method */
/**
 *  is the Item(Matrix) placeable or not in the point
*/
bool Stock::isPlaceable(Point point, Matrix &matrix) { return false; }

/**
 * insert the Item(Matrix) in Point
*/
void Stock::insertItem(Point point, Matrix &matrix) {}