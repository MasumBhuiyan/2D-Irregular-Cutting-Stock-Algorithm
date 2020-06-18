#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "Matrix.hpp"
#include "Point.hpp"

bool isBetter(Matrix &, Matrix &);
Matrix enclosure(int,int,Matrix &, Matrix &);
Matrix mergeItems(Matrix &, Matrix &);
Matrix split(std::vector<Matrix>&,int, int);
void solution();
#endif // ITEM_HPP