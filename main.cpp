#include <iostream>
#include "io.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        double width = 0;
        vector<Item> items = io::readItemsFromInputFile(string(argv[1]), width);
        cout << "Bean width: " << width << endl;
        for (auto p : items)
        {
            p.print();
        }
    }
    else
    {
        cerr << "Argument error" << endl;
        exit(-1);
    }
}