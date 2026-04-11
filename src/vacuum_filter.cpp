#include "vacuum_filter.hpp"
#include <stdexcept>

// Author: Matino Milicevic
VacuumFilter::VacuumFilter(size_t capacity, double fpr)
    : capacity_(capacity), fpr_(fpr) {
    if (fpr <= 0.0 || fpr >= 1.0)
        throw std::invalid_argument("False positive rate must be in (0, 1)");
    
    // Formula from the paper: l = ceil(log2(2 * b * alpha / epsilon))
    fingerprint_len_ = static_cast<size_t>(
        std::ceil(std::log2(2.0 * kSlotsPerBucket * kTargetLoadFactor / fpr)));
    if (fingerprint_len_ < 1)  fingerprint_len_ = 1;
    if (fingerprint_len_ > 32) fingerprint_len_ = 32;

    // Compute 4 alternate ranges (Algorithm 3 from the paper)
    // Smaller ARs => better locality 
    // Larger ARs => higher load factor
    for (int i = 0; i < 4; i++) {
        alt_range_[i] = range_selection(capacity, kTargetLoadFactor, 1.0 - i / 4.0);
    }
    alt_range_[3] *= 2;

    // Compute number of buckets
    size_t L_max = 1;
    for (int i = 0; i < 4; i++)
        if (alt_range_[i] > L_max) L_max = alt_range_[i];

    // Formula from the paper: m = ceil(n / (4 * alpha * L)) * L
    num_buckets_ = static_cast<size_t>(std::ceil(
        static_cast<double>(capacity) / (kSlotsPerBucket * kTargetLoadFactor * L_max))) * L_max;

    // Allocate bucket table
    buckets_.assign(num_buckets_, {});
}

bool VacuumFilter::insert(const std::string& key) {
    // TODO
    (void)key;
    return false;
}

bool VacuumFilter::lookup(const std::string& key) const {
    // TODO
    (void)key;
    return false;
}

bool VacuumFilter::remove(const std::string& key) {
    // TODO
    (void)key;
    return false;
}

double VacuumFilter::load_factor() const {
    return 0.0; // TODO
}

size_t VacuumFilter::size_in_bytes() const {
    return 0; // TODO
}

size_t VacuumFilter::range_selection(size_t n, double alpha, double r) {
    // TODO
    return 1;
}