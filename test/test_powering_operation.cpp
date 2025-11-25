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
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1};
        pcp::BitPCP orig_pcp(bits);
        for (int i = 0; i < 5; ++i) {
            orig_pcp.add_constraint(i, (i + 1) % 5, constraint::BitConstraint::EQUAL);
        }
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        // Check powered variables match neighborhood bits
        for (int i = 0; i < 5; ++i) {
            auto neighbors = orig_pcp.get_neighbors(i, radius);
            const auto &powered_vars = powered.get_variables(i);
            assert(powered_vars.size() == neighbors.size());
            for (size_t j = 0; j < neighbors.size(); ++j) {
                assert(powered_vars[j] == bits[neighbors[j]]);
            }
        }
        // Check constraints for mutual neighbors and original constraints
        for (int i = 0; i < 5; ++i) {
            auto neighbors_i = orig_pcp.get_neighbors(i, radius);
            for (const auto &[adj, pc] : powered.get_constraints(i)) {
                auto neighbors_adj = orig_pcp.get_neighbors(adj, radius);
                // Check mutual neighbor consistency constraints
                for (size_t idx_i = 0; idx_i < neighbors_i.size(); ++idx_i) {
                    int mutual = neighbors_i[idx_i];
                    auto it = std::find(neighbors_adj.begin(), neighbors_adj.end(), mutual);
                    if (it != neighbors_adj.end()) {
                        size_t idx_adj = std::distance(neighbors_adj.begin(), it);
                        bool found = false;
                        for (const auto &[other_idx, cons] : pc.get_constraints(idx_i)) {
                            if (other_idx == idx_adj && cons == constraint::BitConstraint::EQUAL) {
                                found = true;
                                break;
                            }
                        }
                        assert(found);
                    }
                }
                // Check original constraints are mapped
                for (size_t idx_i = 0; idx_i < neighbors_i.size(); ++idx_i) {
                    int orig_var = neighbors_i[idx_i];
                    for (const auto &[orig_adj, orig_constraint] : orig_pcp.get_constraints(orig_var)) {
                        auto it_adj = std::find(neighbors_adj.begin(), neighbors_adj.end(), orig_adj);
                        if (it_adj != neighbors_adj.end()) {
                            size_t idx_adj = std::distance(neighbors_adj.begin(), it_adj);
                            bool found = false;
                            for (const auto &[other_idx, cons] : pc.get_constraints(idx_i)) {
                                if (other_idx == idx_adj && cons == orig_constraint) {
                                    found = true;
                                    break;
                                }
                            }
                            assert(found);
                        }
                    }
                }
            }
        }
    },
    // Test 2: Star graph, center 0, radius 1
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1};
        pcp::BitPCP orig_pcp(bits);
        for (int i = 1; i < 5; ++i) {
            orig_pcp.add_constraint(0, i, constraint::BitConstraint::NOTEQUAL);
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
        for (int i = 1; i < 5; ++i) {
            auto leaf_neighbors = orig_pcp.get_neighbors(i, radius);
            auto center_neighbors = orig_pcp.get_neighbors(0, radius);
            for (const auto &[adj, pc] : powered.get_constraints(0)) {
                if (adj == i) {
                    // For each original constraint (0, i, NOT_EQUAL), check mapping
                    auto it_center = std::find(center_neighbors.begin(), center_neighbors.end(), 0);
                    auto it_leaf = std::find(leaf_neighbors.begin(), leaf_neighbors.end(), i);
                    if (it_center != center_neighbors.end() && it_leaf != leaf_neighbors.end()) {
                        size_t idx_center = std::distance(center_neighbors.begin(), it_center);
                        size_t idx_leaf = std::distance(leaf_neighbors.begin(), it_leaf);
                        bool found = false;
                        for (const auto &[other_idx, constraint] : pc.get_constraints(idx_center)) {
                            if (other_idx == idx_leaf && constraint == constraint::BitConstraint::NOTEQUAL) {
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
    // Test 3: Disconnected graph, radius 1
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1};
        pcp::BitPCP orig_pcp(bits);
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        for (int i = 0; i < 3; ++i) {
            auto neighbors = orig_pcp.get_neighbors(i, radius);
            const auto &powered_vars = powered.get_variables(i);
            assert(powered_vars.size() == neighbors.size());
            for (size_t j = 0; j < neighbors.size(); ++j) {
                assert(powered_vars[j] == bits[neighbors[j]]);
            }
            assert(powered.get_constraints(i).empty());
        }
    },
    // Test 4: Single node, radius 1
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1};
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
    },
    // Test 5: Large chain, just check it doesn't time out
    []() -> void {
        const int N = 1000;
        std::vector<pcp::BitDomain> bits(N, 0);
        pcp::BitPCP orig_pcp(bits);
        for (int i = 0; i < N - 1; ++i) {
            orig_pcp.add_constraint(i, i + 1, constraint::BitConstraint::EQUAL);
        }
        int radius = 10;
        auto powered = core::powering_operation(orig_pcp, radius);
        assert(powered.get_size() == N);
    },
    // Test 6: Large star, just check it doesn't time out
    []() -> void {
        const int N = 100;
        std::vector<pcp::BitDomain> bits(N, 0);
        pcp::BitPCP orig_pcp(bits);
        for (int i = 1; i < N; ++i) {
            orig_pcp.add_constraint(0, i, constraint::BitConstraint::EQUAL);
        }
        int radius = 2;
        auto powered = core::powering_operation(orig_pcp, radius);
        assert(powered.get_size() == N);
    },
    // Test 7: Large star, just check it doesn't time out
    []() -> void {
        const int N = 1000;
        std::vector<pcp::BitDomain> bits(N, 0);
        pcp::BitPCP orig_pcp(bits);
        for (int i = 1; i < N; ++i) {
            orig_pcp.add_constraint(0, i, constraint::BitConstraint::EQUAL);
        }
        int radius = 1;
        auto powered = core::powering_operation(orig_pcp, radius);
        assert(powered.get_size() == N);
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