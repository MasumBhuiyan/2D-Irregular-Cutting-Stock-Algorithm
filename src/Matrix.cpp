#include "Matrix.hpp"
#include "geometry.hpp"
#include "Point.hpp"

int dx[] = {0,0,1,1,1,-1,-1,-1};
int dy[] = {1,-1,0,1,-1,0,1,-1};

/** constructors */
Matrix::Matrix() {}
Matrix::Matrix(int r, int c) : row(r), col(c)
{
    mat.resize(row, std::vector<int>(col, 0));
}

/** methods */
/**
 * Prints matrix on console
*/
void Matrix::print()
{
    std::cout << "dimension: " << row << " x " << col << std::endl;
    for (int i = 0; i < row; i += 1)
    {
        for (int j = 0; j < col; j += 1)
        {
            if (mat[i][j] == 0)
            {
                std::cout << "- ";
            }
            else
            {
                std::cout << mat[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * returns {row, col}
*/
std::pair<int, int> Matrix::getDimension() { return {row, col}; }

/**
 * checks the provided matrix is intersecting or not 
 * if it is placed in Point
*/
bool Matrix::isIntersecting(Point point, Matrix &matrix)
{
    std::pair<int, int> row_col = matrix.getDimension();
    int _row = row_col.first;
    int _col = row_col.second;

    for (int i = point.x; i < std::min(row, (int)point.x + _row); i += 1)
    {
        for (int j = point.y; j < std::min(col, (int)point.y + _col); j += 1)
        {
            if (matrix.mat[i - point.x][j - point.y] and mat[i][j])
            {
                return true;
            }
        }
    }
    return false;
}
/**
 * rotates matrix by 90 degree anti clockwise
*/
Matrix Matrix::rotate90()
{
    int n = std::max(row, col);
    Matrix rotated;
    rotated.row = row;
    rotated.col = col;
    rotated.mat = mat;
    for (int x = 0; x < n / 2; x += 1)
    {
        for (int y = x; y < n - x - 1; y += 1)
        {
            int t = rotated.mat[x][y];
            rotated.mat[x][y] = rotated.mat[y][n - 1 - x];
            rotated.mat[y][n - 1 - x] = rotated.mat[n - 1 - x][n - 1 - y];
            rotated.mat[n - 1 - x][n - 1 - y] = rotated.mat[n - 1 - y][x];
            rotated.mat[n - 1 - y][x] = t;
        }
    }
    return rotated;
}
/**
 * rotates matrix by angle degree anti clockwise
*/
Matrix Matrix::rotate(double angle)
{
    Matrix rotated(std::max(row, col) + 10, std::max(row,col) + 10);
    std::vector<Point> rotatedPoints;
    std::vector<Point> points;
    double minX = 1e9, minY = 1e9;
    angle = geo::DEG2RAD(angle);
    for (int i = 0; i < row; i += 1)
    {
        for (int j = 0; j < col; j += 1)
        {
            if (mat[i][j])
            {
                Point point = Point(i, j).rotate(Point(0, 0), angle);
                points.push_back(Point(i, j));
                rotatedPoints.push_back(point);

                minX = std::min(minX, point.x);
                minY = std::min(minY, point.y);
            }
        }
    }

    for(int i = 0; i < rotatedPoints.size(); i += 1)
    {
        rotatedPoints[ i ].x -= minX - 2;
        rotatedPoints[ i ].y -= minY - 2;
    }


    for (int i = 0; i < rotatedPoints.size(); i += 1)
    {
        int x = (int)(rotatedPoints[i].x + 0.5);
        int y = (int)(rotatedPoints[i].y + 0.5);
        if( x < 0 || x >= rotated.row || y < 0 || y >= rotated.col ) continue;
        rotated.mat[ x ][ y ] = mat[ (int)points[i].x ][ (int)points[i].y ];
    }

    points.clear();
    for(int i = 0; i < rotated.row; i += 1)
    {
        for(int j = 0; j < rotated.col; j += 1)
        {
            int cnt = 0;
            for(int k = 0; k < 8; k += 1)
            {
                int x = i + dx[ k ];
                int y = j + dy[ k ];

                if( x < 0 || x >= rotated.row || y < 0 || y >= rotated.col || rotated.mat[ x ][ y ] == 0 ) continue;
                cnt += 1; 
            }
            if( cnt >= 4 )
            {
                rotated.mat[ i ][ j ] = 1;
            }
        }
    }
    return rotated;
}

/**
 * insert the Item(Matrix) in Point
*/
void Matrix::insertItem(Point point, Matrix &matrix, int indicator)
{
    std::pair<int, int> row_col = matrix.getDimension();
    int _row = row_col.first;
    int _col = row_col.second;

    for (int i = point.x; i < std::min(row, (int)point.x + _row); i += 1)
    {
        for (int j = point.y; j < std::min(col, (int)point.y + _col); j += 1)
        {
            if (matrix.mat[i - point.x][j - point.y])
            {
                mat[i][j] = indicator;
            }
        }
    }
}