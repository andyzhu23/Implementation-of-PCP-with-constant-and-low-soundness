#include "util/disjoint_set_union.hpp"

#include <numeric>

namespace util {

disjoint_set_union::disjoint_set_union(size_t size) : size(size), representative(size) {
    std::iota(representative.begin(), representative.end(), 0);
}

size_t disjoint_set_union::find(size_t x) {
    if (representative[x] != x) {
        representative[x] = find(representative[x]);
    }
    return representative[x];
}

bool disjoint_set_union::merge(size_t x, size_t y) {
    size_t rep_x = find(x);
    size_t rep_y = find(y);
    if (rep_x != rep_y) {
        representative[rep_y] = rep_x;
        return true;
    }
    return false;
}

bool disjoint_set_union::same_set(size_t x, size_t y) {
    return find(x) == find(y);
}

}