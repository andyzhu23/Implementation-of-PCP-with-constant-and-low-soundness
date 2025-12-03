
#include <vector>

#include "util.hpp"

namespace util {

visit_guard::visit_guard(std::vector<bool> &visited, const std::vector<pcp::Variable> &nodes)
    : visited(visited), nodes(nodes) {}

visit_guard::~visit_guard() {
    for (int node : nodes) {
        visited[node] = false;
    }
}

}