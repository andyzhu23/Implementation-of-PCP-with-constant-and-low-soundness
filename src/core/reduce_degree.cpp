#include <chrono>
#include <random>
#include <stdexcept>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"
#include "util.hpp"

namespace core {

pcp::BitPCP reduce_degree(const pcp::BitPCP &pcp, int degree) {
    // generate random seed
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    if (degree < 3) {
        throw std::invalid_argument("degree must be at least 3");
    }
    size_t original_size = pcp.get_size(), new_size = 0;
    std::vector<size_t> sizes, offsets;
    sizes.reserve(original_size);
    offsets.reserve(original_size);
    for (size_t i = 0; i < original_size; ++i) {
        sizes.push_back(pcp.get_constraints(i).size());
        new_size += sizes.back();
        offsets.push_back(new_size - sizes.back());
    }
    pcp::BitPCP reduced_pcp(new_size);
    for (size_t i = 0; i < original_size; ++i) {
        // Connect in a cycle
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            size_t next = offsets[i] + (j + 1) % sizes[i];
            reduced_pcp.add_constraint(curr, next, constraint::BitConstraint::ANY);
            // Set bits to match original variable
            reduced_pcp.set_variable(curr, pcp.get_variable(i));
            reduced_pcp.set_variable(next, pcp.get_variable(i));
        }
        const auto &constraints = pcp.get_constraints(i);
        // Map original constraints
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            pcp::Variable adj = constraints[j].first; // original neighbor index
            if (adj < i) continue; // to avoid double adding
            constraint::BitConstraint con = constraints[j].second;
            int constraint_pos = pcp.get_constraints_indices(i)[j].second;
            size_t adj_new_index = offsets[adj] + constraint_pos;
            reduced_pcp.add_constraint(curr, adj_new_index, con);
        }
        // random edge picker for picking random edges to connect local expander
        util::random_picker<pcp::Variable> rp;
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            rp.add(curr, degree - reduced_pcp.get_constraints(curr).size());
        }
        while (rp.size() > 1) {
            auto [u, v] = rp.pick_two();
            reduced_pcp.add_constraint(u, v, constraint::BitConstraint::ANY);
        }
    }

    return reduced_pcp;
}

}