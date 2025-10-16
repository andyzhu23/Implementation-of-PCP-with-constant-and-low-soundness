/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#include <functional>
#include <iostream>
#include <cassert>
#include <vector>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"
#include "pcp/PoweringPCP.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: 5-node cycle, radius 1
    []() -> void {
        std::vector<bool> bits = {true, false, true, false, true};
        pcp::BitPCP orig_pcp(bits);
        for (int i = 0; i < 5; ++i) {
            orig_pcp.add_constraint(i, (i + 1) % 5, pcp::BitConstraint::UNDEFINED);
        }
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        // Each powered variable should correspond to the neighborhood of each node
        for (int i = 0; i < 5; ++i) {
            auto neighbors = orig_pcp.get_neighbors(i, radius);
            const auto &powered_vars = powered.get_variables(i);
            assert(powered_vars.size() == neighbors.size());
            for (size_t j = 0; j < neighbors.size(); ++j) {
                assert(powered_vars[j] == bits[neighbors[j]]);
            }
        }
        // Check that constraints enforce EQUAL for mutual neighbors
        for (int i = 0; i < 5; ++i) {
            for (const auto &[adj, pc] : powered.get_constraints(i)) {
                // For each constraint, check that the EQUAL constraint exists for mutual neighbors
                auto n1 = orig_pcp.get_neighbors(i, radius);
                auto n2 = orig_pcp.get_neighbors(adj, radius);
                for (int mutual : n1) {
                    if (std::find(n2.begin(), n2.end(), mutual) != n2.end()) {
                        // Find EQUAL constraint for mutual
                        bool found = false;
                        for (const auto &c : pc.get_constraints(std::distance(n1.begin(), std::find(n1.begin(), n1.end(), mutual)))) {
                            if (c.second == pcp::BitConstraint::EQUAL) {
                                found = true;
                                break;
                            }
                        }
                        assert(found);
                    }
                }
            }
        }
    },
    // Test 2: Star graph, center 0, radius 1
    []() -> void {
        std::vector<bool> bits = {true, false, true, false, true};
        pcp::BitPCP orig_pcp(bits);
        for (int i = 1; i < 5; ++i) {
            orig_pcp.add_constraint(0, i, pcp::BitConstraint::NOT_EQUAL);
        }
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        // Center node should see all leaves
        auto center_neighbors = orig_pcp.get_neighbors(0, radius);
        const auto &powered_vars = powered.get_variables(0);
        assert(powered_vars.size() == center_neighbors.size());
        for (size_t j = 0; j < center_neighbors.size(); ++j) {
            assert(powered_vars[j] == bits[center_neighbors[j]]);
        }
        // Each leaf should see itself and center
        for (int i = 1; i < 5; ++i) {
            auto leaf_neighbors = orig_pcp.get_neighbors(i, radius);
            const auto &leaf_vars = powered.get_variables(i);
            assert(leaf_vars.size() == leaf_neighbors.size());
            for (size_t j = 0; j < leaf_neighbors.size(); ++j) {
                assert(leaf_vars[j] == bits[leaf_neighbors[j]]);
            }
        }
        // Check NOT_EQUAL constraint is enforced in powered PCP
        const auto &constraints = powered.get_constraints(0);
        int center_center_position = std::find(center_neighbors.begin(), center_neighbors.end(), 0) - center_neighbors.begin();
        for (int i = 1; i < 5; ++i) {
            bool found_0_i = false;
            bool found_i_0 = false;
            int center_i_position = std::find(center_neighbors.begin(), center_neighbors.end(), i) - center_neighbors.begin();
            auto i_neighbors = orig_pcp.get_neighbors(i, radius);
            int i_center_position = std::find(i_neighbors.begin(), i_neighbors.end(), 0) - i_neighbors.begin();
            int i_i_position = std::find(i_neighbors.begin(), i_neighbors.end(), i) - i_neighbors.begin();
            for (const auto &[adj, pc] : constraints) {
                if (adj == i) {
                    // Should have NOT_EQUAL constraint between center to leaf
                    for (const auto &[j, constraint] : pc.get_constraints(center_center_position)) {
                        if (j == i_i_position && constraint == pcp::BitConstraint::NOT_EQUAL) {
                            found_0_i = true;
                            break;
                        }
                    }
                    // Should have NOT_EQUAL constraint between leaf to center
                    for (const auto &[j, constraint] : pc.get_constraints(center_i_position)) {
                        if (j == i_center_position && constraint == pcp::BitConstraint::NOT_EQUAL) {
                            found_i_0 = true;
                            break;
                        }
                    }
                }
            }
            assert(found_0_i);
            assert(found_i_0);
        }
    },
    // Test 3: Disconnected graph, radius 1
    []() -> void {
        std::vector<bool> bits = {true, false, true};
        pcp::BitPCP orig_pcp(bits);
        // No constraints
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        for (int i = 0; i < 3; ++i) {
            auto neighbors = orig_pcp.get_neighbors(i, radius);
            const auto &powered_vars = powered.get_variables(i);
            assert(powered_vars.size() == neighbors.size());
            for (size_t j = 0; j < neighbors.size(); ++j) {
                assert(powered_vars[j] == bits[neighbors[j]]);
            }
            // No constraints should exist
            assert(powered.get_constraints(i).empty());
        }
    },
    // Test 4: Single node, radius 1
    []() -> void {
        std::vector<bool> bits = {true};
        pcp::BitPCP orig_pcp(bits);
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        auto neighbors = orig_pcp.get_neighbors(0, radius);
        const auto &powered_vars = powered.get_variables(0);
        assert(powered_vars.size() == neighbors.size());
        for (size_t j = 0; j < neighbors.size(); ++j) {
            assert(powered_vars[j] == bits[neighbors[j]]);
        }
        assert(powered.get_constraints(0).empty());
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}