#include "rasterization.hpp"

/**
 * 4 connected directions
*/
int x_dir[] = {0, 0, 1, -1};
int y_dir[] = {1, -1, 0, 0};

/**
 * finds the minimum x and y among the points
 * set new origin to (x, y)
 * translate all the points to (x, y)
*/
void raster::translateToNewOrigin(Polygon &points, double &max_x, double &max_y, Point &seed)
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
    seed.x -= x;
    seed.y -= y;
}

/**
 * finds the [row, col] for the raster
*/
std::pair<int, int> raster::getRasterMatrixDimension(Polygon &polygon, Point &seed)
{
    double max_x = -MAX_X;
    double max_y = -MAX_Y;
    translateToNewOrigin(polygon, max_x, max_y, seed);
    int row = ceil(max_x) + 2;
    int col = ceil(max_y) + 2;
    return {row, col};
}

/**
 * scan converts the line connecting point a, b
*/
void raster::scanConvertLine(Matrix &matrix, Point a, Point b)
{
    // swap a, b if a.x > b.x so that always a.x <= b.x
    if (a.x > b.x)
    {
        std::swap(a, b);
    }

    //always x1 <= x2
    int x1 = (int)(a.x + 0.5);
    int x2 = (int)(b.x + 0.5);
    int y1 = (int)(a.y + 0.5);
    int y2 = (int)(b.y + 0.5);
    int dx = x2 - x1;
    int dy = y2 - y1;

    // m = 0 i.e. horizontal line
    if (dy == 0)
    {
        for (int x = x1, y = y1; x <= x2; x += 1)
        {
            matrix.mat[x][y] = 1;
        }
    }
    // m = infinity i.e. vertical line
    else if (dx == 0)
    {
        if( y1 > y2 )
        {
            std::swap(y1, y2);
        }
        for (int y = y1, x = x1; y <= y2; y += 1)
        {
            matrix.mat[x][y] = 1;
        }
    }
    // 0 < m < 1 i.e. positive slope
    else if (dy > 0)
    {
        int eps = 0;
        for (int x = x1, y = y1; x <= x2; x += 1)
        {
            matrix.mat[x][y] = 1;
            eps += dy;
            if ((eps << 1) >= dx)
            {
                y += 1;
                eps -= dx;
            }
        }
    }
    // m < 0 i.e. negative slope
    else
    {
        int eps = 0;
        for (int x = x1, y = y1; x <= x2; x += 1)
        {
            matrix.mat[x][y] = 1;
            eps += dy;
            if ((eps << 1) <= -dx)
            {
                y -= 1;
                eps += dx;
            }
        }
    }
}

/**
 * fills the inner region of the matrix bounded by the polygon 
*/
void raster::regionfill(Matrix &matrix, int x, int y) 
{
    matrix.mat[ x ][ y ] = 1;
    for(int i = 0; i < 4; i += 1)
    {
        int _x = x + x_dir[ i ];
        int _y = y + y_dir[ i ];

        if( matrix.mat[ _x ][ _y ] == 0 )
        {
            regionfill(matrix, _x, _y);
        }
    }  
}