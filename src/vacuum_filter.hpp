#pragma once
#include <cstdint>
#include <vector>
#include <string>

/**
 * Usage:
 *   VacuumFilter vf(capacity, false_positive_rate);
 *   vf.insert("hello");
 *   bool found = vf.lookup("hello");  // true
 *   bool found2 = vf.lookup("world"); // false
 */
class VacuumFilter {
public:
    VacuumFilter(size_t capacity, double fpr);
    ~VacuumFilter() = default;

    bool insert(const std::string& key);
    bool lookup(const std::string& key) const;
    bool remove(const std::string& key);

    double load_factor() const;
    size_t size_in_bytes() const;

private:
    // Fixed constants (from the paper)
    static constexpr size_t kSlotsPerBucket  = 4;       // b = 4 slots per bucket
    static constexpr double kTargetLoadFactor = 0.95;   // α = 95% table occupancy
    static constexpr size_t kMaxEvictions    = 500;     // max cuckoo eviction steps

    // Filter parameters
    size_t capacity_;          // max number of items
    double fpr_;               // target false positive rate (ε)
    size_t num_buckets_;       // m — total number of buckets in the table
    size_t fingerprint_len_;   // l — fingerprint length in bits

    // Alternate ranges
    size_t alt_range_[4];      // L[0]..L[3]

    // Bucket table
    std::vector<std::array<uint32_t, kSlotsPerBucket>> buckets_;
};
