#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "Matrix.hpp"
#include "Point.hpp"
#include <algorithm>
#include <vector>

bool isBetter(Matrix &, Matrix &);            
Matrix enclosure(int,int,Matrix &, Matrix &);
Matrix mergeItems(Matrix &, Matrix &);
Matrix split(std::vector<Matrix>&,int, int);
Matrix orientItems(std::vector<Matrix>&);
double packingDensity(Matrix &);
#endif // ITEM_HPP