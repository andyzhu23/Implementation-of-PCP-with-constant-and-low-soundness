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
#include "util.hpp"

namespace core {

pcp::PoweringPCP powering_operation(const pcp::BitPCP &pcp, int radius) {
    std::vector<std::vector<pcp::Variable>> neighbors;
    neighbors.reserve(pcp.get_size());
    // Get neighbors for each variable in the original PCP
    for (size_t i = 0; i < pcp.get_size(); ++i) {
        neighbors.push_back(pcp.get_neighbors(i, radius));
    }

    pcp::PoweringPCP result(pcp.get_size());

    // Maps to track indices in the new variable sets
    std::vector<size_t> i_index_map(pcp.get_size(), -1);
    std::vector<size_t> neighbor_index_map(pcp.get_size(), -1);

    for (size_t i = 0; i < pcp.get_size(); ++i) {
        std::vector<pcp::BitDomain> combined_vars;
        combined_vars.reserve(neighbors[i].size());
        std::vector<std::pair<std::pair<int, int>, constraint::BitConstraint>> constraints_to_add;
        // Build i_index_map, and RAII guard to reset after use
        util::index_map_guard<pcp::Variable> i_map_guard(i_index_map, neighbors[i]);
        for (size_t j = 0; j < neighbors[i].size(); ++j) {
            combined_vars.push_back(pcp.get_variable(neighbors[i][j]));
            // Collect constraints from original PCP
            for (const auto &[adj, bc] : pcp.get_constraints(neighbors[i][j])) {
                constraints_to_add.emplace_back(std::make_pair(neighbors[i][j], adj), bc);
            }
        }
        // Set combined variables for the new PCP at index i
        result.set_variables(i, std::move(combined_vars));
        // Add constraints with neighborhood variables

        for (const auto &neighbor : neighbors[i]) {
            if (neighbor == i) continue; // skip self-loop
            // Build neighbor_index_map and RAII guard
            util::index_map_guard<pcp::Variable> neighbor_map_guard(neighbor_index_map, neighbors[neighbor]);
            constraint::PoweringConstraint pc(neighbors[i].size());
            
            // add consistency constraints
            for (const int mutual_neighbor : neighbors[i]) {
                if (neighbor_index_map[mutual_neighbor] != -1) {
                    // mutual neighbor found, must have consistent bit
                    pc.add_constraint(
                        i_index_map[mutual_neighbor], 
                        neighbor_index_map[mutual_neighbor], 
                        constraint::BitConstraint::EQUAL
                    );
                }
            }

            // add original constraints
            for (const auto &[var_pair, bc] : constraints_to_add) {
                if (i_index_map[var_pair.first] != -1 && neighbor_index_map[var_pair.second] != -1) {
                    pc.add_constraint(
                        i_index_map[var_pair.first],
                        neighbor_index_map[var_pair.second],
                        bc
                    );
                }
            }

            // add to result
            result.add_constraint(i, neighbor, std::move(pc));
        }
    }

    return result;
}

}