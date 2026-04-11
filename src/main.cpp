#include <iostream>
#include <string>
#include "vacuum_filter.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input.fasta> <k> <fpr>\n";
        std::cerr << "  input.fasta  — input sequences in FASTA format\n";
        std::cerr << "  k            — k-mer size\n";
        std::cerr << "  fpr          — false positive rate\n";
        return 1;
    }

    std::string input_file = argv[1];
    int k = std::stoi(argv[2]);
    double fpr = std::stod(argv[3]);

    std::cout << "Vacuum Filter — k=" << k << ", fpr=" << fpr << "\n";
    std::cout << "Input: " << input_file << "\n";

    // Test constructor
    size_t capacity = 1000000;
    VacuumFilter vf(capacity, fpr);

    // TODO: read FASTA, extract k-mers, insert into filter, benchmark

    return 0;
}
