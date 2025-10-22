/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#include <chrono>
#include <random>
#include <vector>

#include "pcp/SimplePCP.hpp"
#include "core/core.hpp"
#include "pcp/PoweringPCP.hpp"

namespace core {

pcp::SimplePCP& to_expander(pcp::SimplePCP &pcp, int expanding_coefficient) {
    // generate random seed
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    if (pcp.get_size() <= 1) {
        return pcp; // cannot expand
    }
    // for each node, add `expanding_coefficient` random ANY constraints to other nodes
    // (avoid self-loops)
    std::uniform_int_distribution<int> dist(1, pcp.get_size() - 1);
    // 0 is reserved for self-loop avoidance
    std::vector<int> options(pcp.get_size());
    std::iota(options.begin(), options.end(), 0);
    for (size_t i = 0; i < pcp.get_size(); ++i) {
        for (int j = 0; j < expanding_coefficient; ++j) {
            int target = dist(rng);
            pcp.add_constraint(i, target, constraint::BinaryANY);
        }
        // swap current node to front to avoid self-loop
        std::swap(options[0], options[i]);
    }
    return pcp;
}

}