/*
 * Author: Andy Zhu
 * @date    2025-10-17 16:04:24
 * @version 1.0.0
 */


#include <vector>

#include "util.hpp"

namespace util {

visit_guard::visit_guard(std::vector<bool> &visited, const std::vector<int> &nodes)
    : visited(visited), nodes(nodes) {}

visit_guard::~visit_guard() {
    for (int node : nodes) {
        visited[node] = false;
    }
}

}