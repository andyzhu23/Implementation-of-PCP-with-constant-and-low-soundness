/*
 * Author: Andy Zhu
 * @date    2025-10-17 16:04:24
 * @version 1.0.0
 */

#include <chrono>
#include <random>
#include <stdexcept>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"

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
            reduced_pcp.add_constraint(curr, next, pcp::BinaryConstraint::EQUAL);
            // Set bits to match original variable
            reduced_pcp.set_variable(curr, pcp.get_variable(i));
        }
        const auto &constraints = pcp.get_constraints(i);
        // Map original constraints
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            int adj = constraints[j].first; // original neighbor index
            pcp::BinaryConstraint constraint = constraints[j].second;
            int constraint_pos = pcp.get_constraints_indices(i)[j].second;
            size_t adj_new_index = offsets[adj] + constraint_pos;
            reduced_pcp.add_constraint(curr, adj_new_index, constraint);
            // Fill additional edges to make local expander graph
            if (sizes[i] - 1 > 1) {
                std::uniform_int_distribution<int> dist(1, sizes[i] - 1);
                auto &reduced_constraints = reduced_pcp.get_constraints(curr);
                while (reduced_constraints.size() < degree) {
                    int rand_neighbor = offsets[i] + dist(rng);
                    // avoid self loop, since dist can only be 1, use self-index to set rand_neighbor to offsets[i] + 0
                    if (rand_neighbor == curr) {
                        rand_neighbor = offsets[i];
                    } 
                    reduced_pcp.add_constraint(curr, rand_neighbor, pcp::BinaryConstraint::EQUAL);
                }
            }
        }
    }

    return reduced_pcp;
}

}