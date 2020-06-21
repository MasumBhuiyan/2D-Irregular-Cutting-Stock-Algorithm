#include "Matrix.hpp"

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
    Matrix rotated(row, col);
    std::vector<Point> points, rotatedPoints;

    int minX = 1e9, minY = 1e9;
    for (int i = 0; i < row; i += 1)
    {
        for (int j = 0; j < col; j += 1)
        {
            if (mat[i][j])
            {
                Point point = Point(i, j).rotate(Point(0, 0), angle);
                int x = ceil(point.x);
                int y = ceil(point.y);
                points.push_back(Point(i, j));
                rotatedPoints.push_back(Point(x, y));
                minX = std::min(x, minX);
                minY = std::min(y, minY);
            }
        }
    }
    for (int i = 0; i < points.size(); i += 1)
    {
        int x = rotatedPoints[i].x;
        int y = rotatedPoints[i].y;
        rotated.mat[x - minX][y - minY] = mat[(int)points[i].x][(int)points[i].y];
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