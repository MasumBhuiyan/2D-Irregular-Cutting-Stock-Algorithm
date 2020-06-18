#include "algorithm.hpp"

/**
 * returns true is Matrix a is better choice than Matrix b
*/
bool isBetter(Matrix &a, Matrix &b) {
	int area1 = a.mat.size() * a.mat[ 0 ].size();
	int area2 = b.mat.size() * b.mat[ 0 ].size();
/*	assert(area1 >= 0 && area1 <= (1LL<<31));
	assert(area2 >= 0 && area2 <= (1LL<<31));*/
	return area1 < area2;
}
/**
 * returns min enclosing rectangle(a Matrix) of the two Matrix a, b
*/
Matrix enclosure(int x, int y, Matrix &a, Matrix &b) {
	std::pair<int,int> row_col_a = a.getDimension();
	std::pair<int,int> row_col_b = b.getDimension();

	int row_a = row_col_a.first;
	int row_b = row_col_b.first;
	int col_a = row_col_a.second;
	int col_b = row_col_b.second;

	int r = std::max(row_a, x + row_b);
	int c = std::max(col_a, y + col_b);

	Matrix rec(r, c);
	for(int i = 0; i < row_a; i += 1)
		for(int j = 0; j < col_a; j += 1)
			rec.mat[ i ][ j ] = rec.mat[ i ][ j ] + a.mat[ i ][ j ];
	for(int i = 0; i < row_b; i += 1) {
		for(int j = 0; j < col_b; j += 1) {
			rec.mat[ i + x ][ j + y ] = rec.mat[ i + x ][ j + y ] + b.mat[ i ][ j ];
		}
	}
	return rec;
}
/**
 * merges two Matrix a and b
*/
Matrix mergeItems(Matrix &a, Matrix &b)
{
	std::pair<int,int> row_col = a.getDimension(); 
	int row_a = row_col.first;
	int col_a = row_col.second;

	Matrix mergedItem;
	for(int i = 0; i < row_a; i += 1) 
	{
		for(int j = 0; j < col_a; j += 1) 
		{
			if(!a.isIntersecting(Point(i, j), b)) 
			{
				Matrix rec = enclosure(i, j, a, b);
				if( mergedItem.mat.size() == 0 || isBetter(rec, mergedItem) ) 
				{
					mergedItem = rec;
				}
			} 
		}
	}
	return mergedItem;
}
/**
 * implements merge sort technique
*/
Matrix split(std::vector<Matrix> &Items, int l, int r) 
{	
	if( l == r ) 
	{
		return Items[ l ];
	}
	int m = (l + r) >> 1;
	Matrix lson = split(Items, l, m);
	Matrix rson = split(Items, m + 1, r);
	return mergeItems(lson, rson);
}
/**
 * proposed algorithm driver function
*/
void solution() {}