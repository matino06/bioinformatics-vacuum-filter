#include "vacuum_filter.hpp"
#include <stdexcept>
#include <cmath>

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

    fp_mask_ = (fingerprint_len_ == 32) ? 0xFFFFFFFFu : (1u << fingerprint_len_) - 1u;

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

static constexpr uint32_t kSeedIndex = 0x9747b28cu;  // seed for H(x)
static constexpr uint32_t kSeedFp    = 0x5f3759dfu;  // seed for H'(x)
// H(x)
uint32_t VacuumFilter::hash_to_index(const std::string& key) const {
    uint32_t out;
    MurmurHash3_x86_32(key.data(), static_cast<int>(key.size()), kSeedIndex, &out);
    return out;
}

//H'(x)
uint32_t VacuumFilter::hash_to_fp(const std::string& key) const {
    uint32_t out;
    MurmurHash3_x86_32(key.data(), static_cast<int>(key.size()), kSeedFp, &out);
    uint32_t fp = out & fp_mask_;
    return fp ? fp : 1u;  // 0 is reserved for "empty slot" — map to 1
}

//H(fp)
uint32_t VacuumFilter::hash_fp(uint32_t fp) const {
    uint32_t out;
    MurmurHash3_x86_32(&fp, static_cast<int>(sizeof(fp)), kSeedFp, &out);
    return out;
}

size_t VacuumFilter::index_of(const std::string& key) const {
    uint64_t h = hash_to_index(key);
    return static_cast<size_t>((h * static_cast<uint64_t>(num_buckets_)) >> 32);
}

size_t VacuumFilter::alt_bucket(size_t b, uint32_t fp) const {
    size_t L = alt_range_[fp % 4];
    size_t delta = hash_fp(fp) % L;
    return b ^ delta;
}

size_t VacuumFilter::alt_bucket2(size_t b, uint32_t fp) const {
    size_t delta = hash_fp(fp) % num_buckets_;
    size_t b2 = (b - delta) % num_buckets_;
    b2 = (num_buckets_ - 1 - b2 + delta) % num_buckets_;
    return b2;
}


bool VacuumFilter::insert(const std::string& key) {
    // TODO
    (void)key;
    return false;
}

bool VacuumFilter::lookup(const std::string& key) const {
    uint32_t fp = hash_to_fp(key);
    size_t   b1 = index_of(key);
    size_t   b2;
    capacity_ < pow(2,18) ? b2 = alt_bucket2(b1,fp) : b2 = alt_bucket(b1,fp);

    for (size_t b : {b1, b2}) {
        for (const uint32_t& slot : buckets_[b]) {
            if (slot == fp) return true;
        }
    }
    return false;
}

bool VacuumFilter::remove(const std::string& key) {
    uint32_t fp = hash_to_fp(key);
    size_t   b1 = index_of(key);
    size_t   b2;
    capacity_ < pow(2,18) ? b2 = alt_bucket2(b1,fp) : b2 = alt_bucket(b1,fp);

    for (size_t b : {b1, b2}) {
        for (uint32_t& slot : buckets_[b]) {
            if (slot == fp) { slot = 0; return true; }
        }
    }
    return false;
}

double VacuumFilter::load_factor() const {
    size_t filled = 0;
    for (const auto& bucket : buckets_)
        for (const uint32_t& slot : bucket) {
            if (slot != 0) filled++;
        }     
    return static_cast<double>(filled) / static_cast<double>(num_buckets_ * kSlotsPerBucket);
}

size_t VacuumFilter::size_in_bytes() const {
    return num_buckets_ * kSlotsPerBucket * sizeof(uint32_t);
}

// Author: Matino Milicevic
double VacuumFilter::estimated_max_load(size_t n, size_t c) {
    double avg  = static_cast<double>(n) / static_cast<double>(c);
    return avg + 1.5 * std::sqrt(2.0 * avg * std::log(static_cast<double>(c)));
}

// Author: Matino Milicevic
bool VacuumFilter::load_factor_test(size_t n, double alpha, double r, size_t L) {
    size_t m = static_cast<size_t>(
        std::ceil(static_cast<double>(n) / (kSlotsPerBucket * alpha * L))) * L; // the number of buckets        
    size_t N = static_cast<size_t>(4.0 * r * static_cast<double>(m) * alpha);   // the number of inserted items
    size_t c = m / L;                                                           // the number of chunks

    double P = 0.97 * kSlotsPerBucket * static_cast<double>(L);     // the capacity lower bound of each chunk
    double D = estimated_max_load(N, c);
    return D < P;
}

// Author: Matino Milicevic
size_t VacuumFilter::range_selection(size_t n, double alpha, double r) {
    size_t L = 1;
    while (!load_factor_test(n, alpha, r, L) && L < (1u << 20)) {
        L *= 2;
    }
    return L;
}