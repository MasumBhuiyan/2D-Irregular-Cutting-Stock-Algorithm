#include "rasterization.hpp"

/**
 * 4 & 8 connected directions
*/
int x_dir[] = {0, 0, 1, -1};
int y_dir[] = {1, -1, 0, 0};
int x_dir8[] = {0, -1, -1, -1, 0, 1, 1, 1};
int y_dir8[] = {-1, -1, 0, 1, 1, 1, 0, -1};

/**
 * finds the minimum x and y among the points
 * set new origin to (x, y)
 * translate all the points to (x, y)
*/
void raster::translateToNewOrigin(Polygon &points, double &max_x, double &max_y)
{
    double x = MAX_X;
    double y = MAX_Y;
    for (auto &point : points)
    {
        x = std::min(x, point.x);
        y = std::min(y, point.y);
    }
    for (auto &point : points)
    {
        point.x -= x;
        point.y -= y;
        max_x = std::max(max_x, point.x);
        max_y = std::max(max_y, point.y);
    }
}

/**
 * finds the [row, col] for the raster
 * max_x is the number of rows in the matrix
 * max_y is the number of columns in the matrix
*/
std::pair<int, int> raster::getRasterMatrixDimension(Polygon &polygon)
{
    double max_x = -MAX_X;
    double max_y = -MAX_Y;
    translateToNewOrigin(polygon, max_x, max_y);
    int row = ceil(max_x) + 2;
    int col = ceil(max_y) + 2;
    return {std::max(row, col), std::max(row, col)};
}

/**
 * region filling starts from seed point 
 * returns a point strictly inside the polygon
*/
std::pair<int, int> raster::getSeedPoint(Matrix &matrix, Polygon &polygon)
{
    std::pair<int, int> seed(-1, -1);
    for (int i = 0; i < matrix.row; i++)
    {
        for (int j = 0; j < matrix.col; j++)
        {
            if (matrix.mat[i][j] == 1)
            {
                continue;
            }
            if (polygonal::isPointInsidePolygon(Point(i, j), polygon) == true)
            {
                bool flg = false;
                for (int k = 0; k < 4; k++)
                {
                    int ii = i + x_dir[k];
                    int jj = j + y_dir[k];
                    if (ii >= 0 and ii < matrix.row and jj >= 0 and jj < matrix.col and matrix.mat[ii][jj] == 0)
                    {
                        if (polygonal::isPointInsidePolygon(Point(ii, jj), polygon) == true)
                        {
                            flg = true;
                            break;
                        }
                    }
                }
                if (flg)
                {
                    seed = {i, j};
                    return seed;
                }
            }
        }
    }
    assert(seed.first != -1 and seed.second != -1 and "Failed to find Seed point");
    return seed;
}

/**
 *  Bresenham's Line Algorithm
*/
void drawLineLow(Matrix &matrix, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;

    for (int x = x0, y = y0; x <= x1; x++)
    {
        matrix.mat[x][y] = 1;
        if (D > 0)
        {
            y = y + yi;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}
void drawLineHigh(Matrix &matrix, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int D = 2 * dx - dy;

    for (int y = y0, x = x0; y <= y1; y++)
    {
        matrix.mat[x][y] = 1;
        if (D > 0)
        {
            x = x + xi;
            D = D - 2 * dy;
        }
        D = D + 2 * dx;
    }
}

/**
 * scan converts the line connecting point a, b
 * Bresenham's Line Algorithm
*/
void raster::scanConvertLine(Matrix &matrix, Point a, Point b)
{
    int x1 = (int)(a.x + 0.5);
    int x2 = (int)(b.x + 0.5);
    int y1 = (int)(a.y + 0.5);
    int y2 = (int)(b.y + 0.5);

    int dx = x2 - x1;
    int dy = y2 - y1;

    // m = 0 i.e. horizontal line
    if (dy == 0)
    {
        if (x1 > x2)
        {
            std::swap(x1, x2);
        }
        for (int x = x1, y = y1; x <= x2; x += 1)
        {
            matrix.mat[x][y] = 1;
        }
        return;
    }
    // m = infinity i.e. vertical line
    if (dx == 0)
    {
        if (y1 > y2)
        {
            std::swap(y1, y2);
        }
        for (int y = y1, x = x1; y <= y2; y += 1)
        {
            matrix.mat[x][y] = 1;
        }
        return;
    }

    bool steep = std::fabs(a.y - b.y) < std::fabs(a.x - b.x);
    if (steep)
    {
        if (x1 > x2)
        {
            drawLineLow(matrix, x2, y2, x1, y1);
        }
        else
        {
            drawLineLow(matrix, x1, y1, x2, y2);
        }
    }
    else
    {
        if (y1 > y2)
        {
            drawLineHigh(matrix, x2, y2, x1, y1);
        }
        else
        {
            drawLineHigh(matrix, x1, y1, x2, y2);
        }
    }
}

/**
 * fills the inner region of the matrix bounded by the polygon 
*/
void raster::regionfill(Matrix &matrix, int x, int y)
{
    matrix.mat[x][y] = 1;
    for (int i = 0; i < 4; i += 1)
    {
        int _x = x + x_dir[i];
        int _y = y + y_dir[i];
        if (_x < 0 or _x >= matrix.row or _y < 0 or _y >= matrix.col)
        {
            continue;
        }
        if (matrix.mat[_x][_y] == 0)
        {
            regionfill(matrix, _x, _y);
        }
    }
}