#include "geometry.hpp"
#include <Item.hpp>
#include <Matrix.hpp>
#include <Point.hpp>

void rotate_test1()
{
	std::vector<Item> items;
        items.push_back(Item(4, vector<Point>(
                                    {   Point(0 * 5, 0 * 5),
                                        Point(5 * 5, 0 * 5),
                                        Point(5 * 5, 5 * 5),
                                        Point(0 * 5, 5 * 5)})));
        items.push_back(Item(3, vector<Point>({ Point(0 * 5, 0 * 5),
                                                Point(5 * 5, 0 * 5),
                                                Point(5 * 5, 5 * 5)})));
        items.push_back(Item(8, vector<Point>({ Point(0 * 5, 1 * 5),
                                                Point(2 * 5, 0 * 5),
                                                Point(5 * 5, 0 * 5),
                                                Point(7 * 5, 1 * 5),
                                                Point(7 * 5, 4 * 5),
                                                Point(5 * 5, 5 * 5),
                                                Point(2 * 5, 5 * 5),
                                                Point(0 * 5, 4 * 5)})));
        Matrix matrix = items.back().rasterize(1);
	int angle = 15;
        for(int i = 0; i < 360 / angle; i += 1)
        {
               std::cout << "Matrix at rotation = " << i * angle << "::\n";
               matrix.rotate(i * angle * 1.0).print();
        } 
}

int main()
{
	rotate_test1();
	return 0;
}