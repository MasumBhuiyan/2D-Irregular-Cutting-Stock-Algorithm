#include <bin_packing.hpp>

int main(int argc, char const *argv[])
{
    bin_packing::runDataset("../tests/dataset/", "albano", 12000);
    bin_packing::runDataset("../tests/dataset/", "sample", 20);
    bin_packing::runDataset("../tests/dataset/", "dighe1", 150);
    bin_packing::runDataset("../tests/dataset/", "dagli", 65);
    bin_packing::runDataset("../tests/dataset/", "poly1a", 20);
    bin_packing::runDataset("../tests/dataset/", "blaz", 29);
    bin_packing::runDataset("../tests/dataset/", "jakobs1", 12);
    bin_packing::runDataset("../tests/dataset/", "jakobs2", 8);
    bin_packing::runDataset("../tests/dataset/", "marques", 84);
    bin_packing::runDataset("../tests/dataset/", "shirts", 62);
    bin_packing::runDataset("../tests/dataset/", "trousers", 280);
    return 0;
}
