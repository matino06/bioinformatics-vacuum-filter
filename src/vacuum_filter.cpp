#include "vacuum_filter.hpp"
#include <stdexcept>

VacuumFilter::VacuumFilter(size_t capacity, double fpr)
    : capacity_(capacity), fpr_(fpr) {
    if (fpr <= 0.0 || fpr >= 1.0)
        throw std::invalid_argument("False positive rate must be in (0, 1)");
    // TODO: allocate filter buckets
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
