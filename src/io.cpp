#include "io.hpp"

/** 
 * reads items from file
 * returns vector<Item> (list if items)
*/
vector<Item> io::readItemsFromInputFile(string filename, double &width)
{
    cout << "Opening file: " << filename << " ....." << endl;
    ifstream file(filename);
    if (file.fail())
    {
        cerr << "File does not exists" << endl;
        exit(-1);
    }
    cout << "Reading file..." << endl;

    /* width of the bean */
    file >> width;

    /* number of Items */
    int numberOfItems;
    file >> numberOfItems;
    vector<Item> Items(numberOfItems);

    /* input all the Items */
    for (auto &item : Items)
    {
        item.read(file);
    }
    cout << "Reading complete" << endl;
    file.close();
    return Items;
}