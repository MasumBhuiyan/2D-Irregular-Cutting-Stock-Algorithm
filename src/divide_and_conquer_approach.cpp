#include "divide_and_conquer_approach.hpp"

std::pair<double, double> approach1::minAreaRectangle(Item &item)
{
    double min_x = INF, max_x = -INF;
    double min_y = INF, max_y = -INF;

    for (Point vertex : item.vertices)
    {
        min_x = std::min(min_x, vertex.x);
        min_y = std::min(min_y, vertex.y);
        max_x = std::max(max_x, vertex.x);
        max_y = std::max(max_y, vertex.y);
    }
    double l = max_x - min_x;
    double w = max_y - min_y;
    return {l, w};
}

Item approach1::mergeHeuristic1(Item &item1, Item &item2)
{
    Item mergedItem;

    return mergedItem;
}

ItemState approach1::mergeItem(ItemState &itemState1, ItemState &itemState2)
{
    double angle = 90;
    double minArea = INF, length = INF;

    Item item1, item2, mergedItem;

    if (itemState1.second >= itemState2.second)
    {
        item1 = itemState1.first;
        item2 = itemState2.first;
    }
    else
    {
        item1 = itemState2.first;
        item2 = itemState1.first;
    }

    for (int i = 0; i < (360 / (int)angle); i++)
    {
        Item tmpMergedItem = mergeHeuristic1(item1, item2);
        auto minRectangle = minAreaRectangle(tmpMergedItem);
        double tmpArea = minRectangle.first * minRectangle.second;
        if (tmpArea < minArea)
        {
            mergedItem = tmpMergedItem;
            minArea = tmpArea;
            length = minRectangle.first;
        }
        else if (tmpArea == minArea and length > minRectangle.first)
        {
            mergedItem = tmpMergedItem;
            length = minRectangle.first;
        }
        item1 = item1.rotate(geo::DEG2RAD(angle), Point(0, 0));
    }

    return {mergedItem, minArea};
}

ItemState approach1::split(std::vector<Item> &items, int left, int right)
{
    if (left == right)
    {
        auto minRectangle = minAreaRectangle(items[left]);
        return {items[left], minRectangle.first * minRectangle.second};
    }
    int mid = left + (right - left) / 2;
    ItemState resultLeft = split(items, left, mid);
    ItemState resultRight = split(items, mid + 1, right);

    return mergeItem(resultLeft, resultRight);
}

void approach1::solution(std::vector<Item> &items)
{
    ItemState result = split(items, 0, items.size() - 1);
}