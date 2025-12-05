
#ifndef UTIL_HPP
#define UTIL_HPP

#include <chrono>
#include <random>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "pcp/SimplePCP.hpp"

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

class visit_guard {
public:
    visit_guard(std::vector<bool> &visited, const std::vector<pcp::Variable> &nodes);
    
    ~visit_guard();

private:
    std::vector<bool> &visited;
    const std::vector<pcp::Variable> &nodes;
};

template <typename T>
class random_picker {
public:
    random_picker() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    void add(const T &item, int count) {
        if (count <= 0) return;
        if (counts.find(item) == counts.end()) {
            counts[item] = 0;
            items.push_back(item);
        }
        counts[item] += count;
    }

    std::pair<T, T> pick_two() {
        if (items.size() < 2) {
            throw std::out_of_range("random_picker: no items to pick from");
        }
        std::uniform_int_distribution<size_t> dist(0, items.size() - 1);
        std::uniform_int_distribution<size_t> dist2(0, items.size() - 2);
        size_t idx = dist(rng);
        T item1 = items[idx];
        std::swap(items[idx], items.back());
        if (--counts[item1] == 0) {
            items.pop_back();
            counts.erase(item1);
        }
        size_t idx2 = dist2(rng);
        while (idx2 == idx) {
            idx2 = dist2(rng);
        }
        T item2 = items[idx2];
        std::swap(items[idx2], items.back());
        if (--counts[item2] == 0) {
            items.pop_back();
            counts.erase(item2);
        }
        return {item1, item2};
    }

    size_t size() const {
        return items.size();
    }
private:
    std::mt19937 rng;
    std::unordered_map<T, size_t> counts;
    std::vector<T> items;
};

}

#endif