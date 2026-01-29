#ifndef DISJOINT_SET_UNION_HPP
#define DISJOINT_SET_UNION_HPP

#include <cstddef>
#include <vector>

namespace util {

class disjoint_set_union {
public:
    disjoint_set_union(size_t size);

    size_t find(size_t x);

    bool merge(size_t x, size_t y);

    bool same_set(size_t x, size_t y);

private:
    size_t size;
    std::vector<size_t> representative;
};

}

#endif