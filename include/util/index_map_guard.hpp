#ifndef INDEX_MAP_GUARD_HPP
#define INDEX_MAP_GUARD_HPP

#include <vector>
#include <stdexcept>

namespace util {

template <typename T>
class index_map_guard {
public:
    index_map_guard(std::vector<size_t> &index_map, const std::vector<T> &orig)
         : index_map(index_map), orig(orig) {
        for (size_t i = 0; i < orig.size(); ++i) {
            if (orig[i] >= index_map.size()) {
                throw std::out_of_range("index_map_guard: orig contains out-of-range index");
            }
            index_map[orig[i]] = i;
        }
    }

    ~index_map_guard() {
        for (size_t x : orig) {
            index_map[x] = -1;
        }
    }
    
private:
    std::vector<size_t> &index_map;
    const std::vector<T> &orig;
};

}

#endif