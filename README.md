# Vacuum Filter

Implementation of the **Vacuum Filter** — a space-efficient probabilistic data structure for approximate set membership queries, based on [Wang et al. (2020)](https://www.vldb.org/pvldb/vol13/p197-wang.pdf).

> Project for the course **[Bioinformatics 1](https://www.fer.unizg.hr/predmet/bio1)** at the Faculty of Electrical Engineering and Computing, University of Zagreb (FER), academic year 2025/26.

---

## Authors

| Name             | GitHub                                   |
| ---------------- | ---------------------------------------- |
| Matino Miličević | [@matino06](https://github.com/matino06) |
| Amar Omeragić    | [@devaamar](https://github.com/devaamar) |

---

## About

The Vacuum Filter is an approximate membership query (AMQ) data structure — similar to a Bloom filter or Cuckoo filter, but more space-efficient. It answers the question _"is this element in the set?"_ with a controllable false-positive rate and no false negatives.

This implementation focuses on k-mer indexing for genomic data, with benchmarks on:

- Synthetic datasets (sequence lengths: 10³–10⁷)
- _E. coli_ genome (real data)

K-mer sizes tested: **k = 10, 20, 50, 100, 200**

---

## Build

Requirements: `cmake >= 3.16`, `g++ >= 9` (C++17)

```bash
git clone https://github.com/matino06/bioinf1-vacuum-filter.git
cd bioinf1-vacuum-filter
mkdir build && cd build
cmake ..
make
```

---

## Usage

```bash
./vacuum_filter <input.fasta> <k> <fpr>
```

- `<input.fasta>` — input genome/sequences in FASTA format
- `<k>` — k-mer size (e.g. 20)
- `<fpr>` — desired false positive rate (e.g. 0.01)

---

## Project Structure

```
vacuum-filter/
├── src/
│   ├── main.cpp
│   ├── vacuum_filter.hpp
│   └── vacuum_filter.cpp
├── tests/
│   └── test_basic.cpp
├── data/              # test input files (not tracked by git)
├── CMakeLists.txt
└── README.md
```

---

## References

- Wang, M. et al. (2020). _Vacuum Filters: More Space-Efficient and Faster Replacement for Bloom and Cuckoo Filters_. VLDB 2020. https://doi.org/10.14778/3372716.3372729
- Bioinformatika 1 course materials, FER Zagreb

---

## License

This project is licensed under the [MIT License](LICENSE).
