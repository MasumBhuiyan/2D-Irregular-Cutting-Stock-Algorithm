#include <bin_packing.hpp>

int main(int argc, char const *argv[])
{
    bin_packing::runDataset("../tests/dataset/", "trousers", 300);
    bin_packing::runDataset("../tests/dataset/", "blaz", 29);
    bin_packing::runDataset("../tests/dataset/", "jakobs1", 12);
    bin_packing::runDataset("../tests/dataset/", "jakobs2", 8);
    bin_packing::runDataset("../tests/dataset/", "marques", 84);
    bin_packing::runDataset("../tests/dataset/", "shirts", 62);
    return 0;
}