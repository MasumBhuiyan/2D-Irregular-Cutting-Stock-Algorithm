#include "divide_and_conquer_approach.hpp"

/**
 * rotates the items according to the item states
 * rasterize the items in a matrix
*/
Matrix DnCApproach::rasterItems(
    std::vector<Item> &items,
    std::vector<ItemState> &itemStates,
    int rasterDimension)
{
    Matrix raster = Matrix(rasterDimension, rasterDimension);
    for (ItemState itemState : itemStates)
    {
        int itemId = itemState.itemId;
        double rotationAngle = itemState.rotatedBy;
        Point pivot = itemState.pivot;

        Item rotatedItem = items[itemId].rotate(rotationAngle, {0, 0});
        Matrix rasterItem = rotatedItem.rasterize();
        raster.insertItem(pivot, rasterItem, itemId + 1); // indicator
    }
    return raster;
}
/**
 * returns min enclosing rectangle(a Matrix) of the two Matrix a, b
*/
double findEnclosingRectangleArea(int x, int y, Matrix &a, Matrix &b) {
    std::pair<int,int> row_col_a = a.getDimension();
    std::pair<int,int> row_col_b = b.getDimension();

    int row_a = row_col_a.first;
    int row_b = row_col_b.first;
    int col_a = row_col_a.second;
    int col_b = row_col_b.second;

    int r = std::max(row_a, x + row_b);
    int c = std::max(col_a, y + col_b);

    return r * c * 1.0;
}
/**
 * merge two raster and returns minimum enclosing rectangle area and
 * new pivot of the second raster
*/
std::pair<double, Point> DnCApproach::mergeItemToFindMinEnclosingRectangleArea(
    Matrix &a, Matrix &b)
{
    std::pair<int,int> row_col = a.getDimension(); 
    int row_a = row_col.first;
    int col_a = row_col.second;

    double minArea = DnCApproach::INF;
    Point pivot;
    for(int i = 0; i < row_a; i += 1) 
    {
        for(int j = 0; j < col_a; j += 1) 
        {
            if(!a.isIntersecting(Point(i, j), b)) 
            {
                double enclosingRectangleArea = findEnclosingRectangleArea(i, j, a, b);
                if( minArea > enclosingRectangleArea ) 
                {
                    minArea = enclosingRectangleArea;
                    pivot = Point(i, j);
                }
            } 
        }
    }
    return {minArea, pivot};
}

/**
 * merge two set of items by rastering them 
*/
std::pair<double, Point> DnCApproach::mergeItemsSet(
    std::vector<Item> &items,
    std::vector<ItemState> &leftHalf,
    std::vector<ItemState> &rightHalf)
{
    Matrix rasterLeftHalf = rasterItems(items, leftHalf);
    Matrix rasterRightHalf = rasterItems(items, rightHalf);
    return mergeItemToFindMinEnclosingRectangleArea(rasterLeftHalf, rasterLeftHalf);
}

/**
 * add rotation angle to ItemState's rotation
 * rotate the ItemState's pivot 
*/
std::vector<ItemState> DnCApproach::rotateItemStates(
    std::vector<ItemState> &itemStates,
    double rotationAngle)
{
    std::vector<ItemState> rotateItemStates = itemStates;
    for (ItemState &itemState : rotateItemStates)
    {
        itemState.rotatedBy += rotationAngle;
        itemState.pivot.rotate({0, 0}, geo::DEG2RAD(rotationAngle));
        itemState.pivot = {std::floor(itemState.pivot.x + 0.5), std::floor(itemState.pivot.y + 0.5)}; // pivot approximation
    }
    return rotateItemStates;
}

/**
 * returns the best orientation by rotating leftHalf and rightHalf
 * with different angles and checking which orientation provides the minimum
 * area rectangle that covers both set of Items
*/
std::vector<ItemState> DnCApproach::findBestOrientation(
    std::vector<Item> &items,
    std::vector<ItemState> &leftHalf,
    std::vector<ItemState> &rightHalf)
{
    double currentBest = DnCApproach::INF;
    std::vector<ItemState> bestOrientation;

    for (double r1 : DnCApproach::rotations)
    {
        std::vector<ItemState> rotatedLeft = rotateItemStates(leftHalf, r1);
        for (double r2 : DnCApproach::rotations)
        {
            std::vector<ItemState> rotatedRight = rotateItemStates(rightHalf, r2);
            std::pair<double, Point> mergeResult = mergeItemsSet(items, rotatedLeft, rotatedRight);
            double minimumEnclosingRectangleArea = mergeResult.first;
            Point pivotOfRightHalf = mergeResult.second;
            if (minimumEnclosingRectangleArea < currentBest)
            {
                currentBest = minimumEnclosingRectangleArea;
                for (ItemState &itemState : rightHalf)
                {
                    itemState.pivot = itemState.pivot + pivotOfRightHalf;
                }
                bestOrientation = leftHalf;
                bestOrientation.insert(bestOrientation.end(), rightHalf.begin(), rightHalf.end());
            }
        }
    }

    return bestOrientation;
}

/**
 * similar to Merge Sort
 * in each step it merges two set of Items by rotating different angles
 * and returns the best orientation
 * best orientation is the orientation of Items that can be enclosed with 
 * the minimum area rectangle 
*/
std::vector<ItemState> DnCApproach::divideAndConquer(std::vector<Item> &items, int left, int right)
{
    if (left == right)
    {
        return {ItemState(left, 0, items[left].pivotPoint)};
    }
    int mid = (left + right) >> 1;
    auto leftHalf = divideAndConquer(items, left, mid);
    auto rightHalf = divideAndConquer(items, mid + 1, right);

    auto bestOrientation = findBestOrientation(items, leftHalf, rightHalf);
    return bestOrientation;
}

/**
 * given a list of Items (at max 10 Items)
 * an orientation ot the Items is generated by divideAndConquer() method
 * the orientation is placed on a stock
 * calculates the packing density
*/
void DnCApproach::solution(std::vector<Item> &items)
{
    int numberOfItems = items.size();
    std::vector<ItemState> orientation = divideAndConquer(items, 0, numberOfItems - 1);
}