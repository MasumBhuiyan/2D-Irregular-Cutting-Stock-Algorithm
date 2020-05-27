#ifndef SORT_H
#define SORT_H

#include "polygon.h"
using namespace::std;

double enclosingRectanleArea(polygon);
vector<polygon> orient(polygon);
void sort(vector<polygon>&);
#endif // SORT_H
