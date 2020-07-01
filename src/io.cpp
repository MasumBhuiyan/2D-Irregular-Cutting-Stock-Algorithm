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
    vector<Item> items;

    /* input all the Items */
    for (int i = 0; i < numberOfItems; i++)
    {
        Item item;
        int quantity;
        file >> quantity;
        item.read(file);
        for (int j = 0; j < quantity; j++)
        {
            items.push_back(item);
        }
    }
    cout << "Reading complete" << endl;
    file.close();
    return items;
}