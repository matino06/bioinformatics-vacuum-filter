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
    size_t capacity_;
    double fpr_;
};
