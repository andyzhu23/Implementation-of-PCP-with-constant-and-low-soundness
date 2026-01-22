#include <chrono>
#include <random>
#include <vector>

#include "util/disjoint_set_union.hpp"
#include "pcp/BitPCP.hpp"
#include "core/core.hpp"
#include "constants.hpp"

namespace core {

pcp::BitPCP& to_expander(pcp::BitPCP &pcp, int expanding_coefficient) {
    // generate random seed
    if (pcp.get_size() <= 1) {
        return pcp; // cannot expand
    }

    // connect all disjoint sets in a ring
    util::disjoint_set_union dsu(pcp.get_size());
    for (const auto &[u, v, c] : pcp.get_constraints_list()) {
        dsu.merge(u, v);
    }
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        pcp::Variable j = (i + 1) % pcp.get_size();
        if (dsu.merge(i, j)) {
            pcp.add_constraint(i, j, constraint::BitConstraint::ANY);
        }
    }

    // for each node, add `expanding_coefficient` random ANY constraints to other nodes
    // (avoid self-loops)
    std::uniform_int_distribution<int> dist(1, pcp.get_size() - 1);
    // 0 is reserved for self-loop avoidance
    std::vector<int> options(pcp.get_size());
    std::iota(options.begin(), options.end(), 0);
    for (size_t i = 0; i < pcp.get_size(); ++i) {
        for (int j = 0; j < expanding_coefficient; ++j) {
            int target = dist(constants::RANDOM_SEED);
            pcp.add_constraint(i, target, constraint::BitConstraint::ANY);
        }
        // swap current node to front to avoid self-loop
        std::swap(options[0], options[i]);
    }
    return pcp;
}

}