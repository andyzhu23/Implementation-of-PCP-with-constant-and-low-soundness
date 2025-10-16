/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#include <chrono>
#include <random>
#include <vector>

#include "pcp/BitPCP.hpp"
#include "core/core.hpp"
#include "pcp/PoweringPCP.hpp"

namespace core {

pcp::PoweringPCP powering_operation(const pcp::BitPCP &pcp, int radius) {
    std::vector<std::vector<int>> neighbors;
    neighbors.reserve(pcp.get_size());
    // Get neighbors for each variable in the original PCP
    for (size_t i = 0; i < pcp.get_size(); ++i) {
        neighbors.push_back(pcp.get_neighbors(i, radius));
    }

    pcp::PoweringPCP result(pcp.get_size());

    // Maps to track indices in the new variable sets
    std::vector<size_t> i_index_map(pcp.get_size(), -1);
    std::vector<size_t> adjacent_index_map(pcp.get_size(), -1);

    for (size_t i = 0; i < pcp.get_size(); ++i) {
        std::vector<bool> combined_vars;
        combined_vars.reserve(neighbors[i].size());
        for (size_t j = 0; j < neighbors[i].size(); ++j) {
            combined_vars.push_back(pcp.get_bit(neighbors[i][j]));
            // Build i_index_map
            i_index_map[neighbors[i][j]] = j;
        }
        // Set combined variables for the new PCP at index i
        result.set_variables(i, std::move(combined_vars));
        // Add constraints with adjacent variables
        for (const auto &[adjacent, constraint] : pcp.get_constraints(i)) {
            // Build adjacent_index_map
            for (size_t k = 0; k < neighbors[adjacent].size(); ++k) {
                adjacent_index_map[neighbors[adjacent][k]] = k;
            }
            pcp::PoweringConstraint pc(neighbors[i].size());
            
            // add consistency constraints
            for (const int mutual_neighbor : neighbors[i]) {
                if (adjacent_index_map[mutual_neighbor] != -1) {
                    // mutual neighbor found, must have consistent bit
                    pc.add_constraint(
                        i_index_map[mutual_neighbor], 
                        adjacent_index_map[mutual_neighbor], 
                        pcp::BitConstraint::EQUAL
                    );
                }
            }

            // add original constraint
            pc.add_constraint(i_index_map[i], adjacent_index_map[adjacent], constraint);
            pc.add_constraint(i_index_map[adjacent], adjacent_index_map[i], constraint);

            // add to result
            result.add_constraint(i, adjacent, std::move(pc));

            // reset adjacent_index_map
            for (const int neighbor : neighbors[adjacent]) {
                adjacent_index_map[neighbor] = -1;
            }
        }
        // Reset i_index_map
        for (const int neighbor : neighbors[i]) {
            i_index_map[neighbor] = -1;
        }
    }

    return result;
}

}