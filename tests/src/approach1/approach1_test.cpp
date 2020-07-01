#include <iostream>
#include <chrono>
using namespace std::chrono;

#include <io.hpp>
#include <divide_and_conquer_approach.hpp>

int main()
{
    auto start = high_resolution_clock::now();

    double width = 0;

    auto items = io::readItemsFromInputFile("../../tests/dataset/sample.txt", width);
    //auto items = io::readItemsFromInputFile("../../tests/dataset/albano.txt", width);
    //auto items = io::readItemsFromInputFile("../../tests/dataset/blaz.txt", width);
    //auto items = io::readItemsFromInputFile("../../tests/dataset/shirts.txt", width); //items size 99
    //auto items = io::readItemsFromInputFile("../../tests/dataset/trousers.txt", width); // items size 64
    //auto items = io::readItemsFromInputFile("../../tests/dataset/marques.txt", width);
    //auto items = io::readItemsFromInputFile("../../tests/dataset/dagli.txt", width);
    //auto items = io::readItemsFromInputFile("../../tests/dataset/jakobs1.txt", width);
    //auto items = io::readItemsFromInputFile("../../tests/dataset/jakobs2.txt", width);

    std::cout << "Items: " << std::endl;
    for (Item item : items)
    {
        polygonal::printPolygon(item.vertices);
    }

    // approach1::solution(items);
    auto tmp = approach1::mergeHeuristic1(items[1], items[2]);
    tmp.print();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() / 1000000.0 << " seconds" << std::endl;
    return 0;
}