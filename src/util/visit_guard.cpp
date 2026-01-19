#include "util/visit_guard.hpp"

namespace util {

visit_guard::visit_guard(std::vector<bool> &visited, const std::vector<pcp::Variable> &nodes)
    : visited(visited), nodes(nodes) {}

visit_guard::~visit_guard() {
    for (const pcp::Variable &node : nodes) {
        visited[static_cast<size_t>(node)] = false;
    }
}

}