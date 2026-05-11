#include <iostream>
#include <string>
#include <fstream>
#include "vacuum_filter.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input.fasta> <k> <fpr>\n";
        cerr << "  input.fasta  — input sequences in FASTA format\n";
        cerr << "  k            — k-mer size\n";
        cerr << "  fpr          — false positive rate\n";
        return 1;
    }

    string input_file = argv[1];
    int k = stoi(argv[2]);
    double fpr = stod(argv[3]);

    cout << "Vacuum Filter — k=" << k << ", fpr=" << fpr << "\n";
    cout << "Input: " << input_file << "\n";

    // Test constructor
    size_t capacity = 1000000;
    VacuumFilter vf(capacity, fpr);

    // TODO: read FASTA, extract k-mers, insert into filter, benchmark

    ifstream file(input_file);

    if (!file.is_open()) {
        cerr << "Greška pri otvaranju datoteke\n";
        return 1;
    }

    string line;
    string sequence;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '>') {
            continue;
        }
        sequence += line;
    }

    file.close();

    for (size_t i = 0; i + k <= sequence.size(); i++) {

        string kmer = sequence.substr(i, k);
        vf.insert(kmer);
    }

    return 0;
}
