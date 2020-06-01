#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Item.hpp"

using namespace std;

namespace io
{
vector<Item> readItemsFromInputFile(string, double &); // reads items from file
};

#endif // IO_HPP