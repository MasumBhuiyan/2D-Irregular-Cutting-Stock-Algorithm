#ifndef IO_HPP
#define IO_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "geometry.hpp"

using namespace std;

namespace io
{
vector<polygon> readInputFile(string filename, double &width);
};

#endif // IO_HPP