#include "algorithm.hpp"

/**
 * returns true is Matrix a is better choice than Matrix b
*/
bool isBetter(Matrix &a, Matrix &b) {
	int area1 = a.mat.size() * a.mat[ 0 ].size();
	int area2 = b.mat.size() * b.mat[ 0 ].size();
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

	Matrix rec(std::max(r, c), std::max(r, c));
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
Matrix split(std::vector<Matrix> &items, int l, int r) 
{	
	if( l == r ) 
	{
		return items[ l ];
	}
	
	int m = (l + r) >> 1;
	Matrix a = split(items, l, m);
	Matrix b = split(items, m + 1, r);
	Matrix best = mergeItems(a, b);

	double angle = 30;
	int rot = 360 / angle;

	for(int i = 0; i < rot; i += 1)
	{
		a = a.rotate(angle);
		for(int j = 0; j < rot; j += 1) 
		{
			b = b.rotate(angle);
			Matrix c = mergeItems(a, b);
			if( isBetter(c, best) == true ) 
			{
				best = c;
			}
		}
	}
	return best;
}
/**
 * return packing density
*/
double packingDensity(Matrix &matrix)
{
	double PD = 0;
	int r1 = 0;
	int r2 = matrix.row - 1;
	int c1 = 0;
	int c2 = matrix.col - 1;

	int rowFlag = 1;
	while( r1 < matrix.row && rowFlag ) 
	{
		for(int i = 0; i < matrix.col; i += 1)
		{
			if( matrix.mat[ r1 ][ i ] )
			{
				rowFlag = 0;
				break;
			}
		}
		if( rowFlag ) 
		{
			r1 += 1;
		}
	}
	rowFlag = 1;
	while( r2 > r1 && rowFlag ) 
	{
		for(int i = 0; i < matrix.col; i += 1)
		{
			if( matrix.mat[ r2 ][ i ] )
			{
				rowFlag = 0;
				break;
			}
		}
		if( rowFlag ) 
		{
			r2 -= 1;
		}
	}

	int colFlag = 1;
	while( c1 < matrix.col && colFlag ) 
	{
		for(int i = 0; i < matrix.row; i += 1)
		{
			if( matrix.mat[ i ][ c1 ] )
			{
				colFlag = 0;
				break;
			}
		}
		if( colFlag ) 
		{
			c1 += 1;
		}
	}
	colFlag = 1;
	while( c2 > c1 && colFlag ) 
	{
		for(int i = 0; i < matrix.row; i += 1)
		{
			if( matrix.mat[ i ][ c2 ] )
			{
				colFlag = 0;
				break;
			}
		}
		if( colFlag ) 
		{
			c2 -= 1;
		}
	}

	for(int i = r1; i <= r2; i += 1)
	{
		for(int j = c1; j <= c2; j += 1)
		{
			if( matrix.mat[ i ][ j ] )
			{
				PD += 1;
			}
		}
	}
	return (PD / ((r2 - r1+1) * (c2-c1+1))) * 100.0;
}
/**
 * orients items in different orders and permutations
*/
Matrix orientItems(std::vector<Matrix> &items) 
{
	int n = items.size();
	std::vector<int> p(n);
	for(int i = 0; i < n; i += 1)
	{
		p[ i ] = i;
	}

	Matrix stock = split(items, 0, items.size() - 1);
	double bestPD = packingDensity(stock);
	while( std::next_permutation(p.begin(), p.end()) ) 
	{
		std::vector<Matrix> tempItems;
		for(int i : p)
		{
			tempItems.push_back(items[ i ]);
		}

		Matrix tempStock = split(tempItems, 0, tempItems.size() - 1);
		double PD = packingDensity(tempStock);
		if( bestPD < PD )
		{
			stock = tempStock;
			bestPD = PD;
		}
	}
	stock.print();
	std::cout << "Best Packing Density = " << bestPD << "%" << "\n";
	return stock;
}