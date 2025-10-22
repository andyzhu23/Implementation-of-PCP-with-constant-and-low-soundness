/*
 * Author: Andy Zhu
 * @date    2025-10-17 16:12:59
 * @version 1.0.0
 */

#include <functional>
#include <iostream>
#include <cassert>
#include <vector>

#include "core/core.hpp"
#include "pcp/SimplePCP.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: 3-node cycle, degree 3
    []() -> void {
        std::vector<int> bits = {1, 0, 1};
        pcp::SimplePCP orig_pcp(bits);
        for (int i = 0; i < 3; ++i) {
            orig_pcp.add_constraint(i, (i + 1) % 3, constraint::BinaryEQUAL);
        }
        int degree = 3;
        auto reduced = core::reduce_degree(orig_pcp, degree);
        // Check new size
        assert(reduced.get_size() == 6);
        // Check bits are preserved
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                assert(reduced.get_variable(i * 2 + j) == bits[i]);
            }
        }
        // Check cycle constraints
        for (int i = 0; i < 6; ++i) {
            int next = (i % 2 == 0) ? i + 1 : i - 1;
            if (next < 0) next += 2;
            if (next >= 6) next -= 2;
            bool found = false;
            for (const auto &[adj, c] : reduced.get_constraints(i)) {
                if (adj == next && c == constraint::BinaryEQUAL) found = true;
            }
            assert(found);
        }
        // Check original constraints mapped
        for (int i = 0; i < 3; ++i) {
            auto orig_constraints = orig_pcp.get_constraints(i);
            for (size_t j = 0; j < orig_constraints.size(); ++j) {
                int adj = orig_constraints[j].first;
                constraint::BinaryConstraint c = orig_constraints[j].second;
                int reduced_idx = i * 2 + j;
                int adj_reduced_idx_offset = adj * 2;
                bool found = false;
                for (const auto &[a, cc] : reduced.get_constraints(reduced_idx)) {
                    if (a - adj_reduced_idx_offset < 2 && cc == c) found = true;
                }
                assert(found);
            }
        }
    },
    // Test 2: Star graph, degree 4
    []() -> void {
        std::vector<int> bits = {1, 0, 1, 0, 1};
        pcp::SimplePCP orig_pcp(bits);
        for (int i = 1; i < 5; ++i) {
            orig_pcp.add_constraint(0, i, constraint::BinaryNOTEQUAL);
        }
        int degree = 4;
        auto reduced = core::reduce_degree(orig_pcp, degree);
        // Check new size
        assert(reduced.get_size() == 8);
        // Check bits are preserved
        for (int i = 0; i < 5; ++i) {
            for (size_t j = 0; j < orig_pcp.get_constraints(i).size(); ++j) {
                int idx = (i == 0) ? j : 4 + (i - 1);
                assert(reduced.get_variable(idx) == bits[i]);
            }
        }
        // Check cycle constraints for center
        for (int i = 0; i < 4; ++i) {
            int next = (i + 1) % 4;
            bool found = false;
            for (const auto &[adj, c] : reduced.get_constraints(i)) {
                if (adj == next && c == constraint::BinaryEQUAL) found = true;
            }
            assert(found);
        }
        // Check original constraints mapped
        for (int i = 1; i < 5; ++i) {
            int idx = 4 + (i - 1);
            bool found = false;
            for (const auto &[adj, c] : reduced.get_constraints(idx)) {
                if (adj < 4 && c == constraint::BinaryNOTEQUAL) found = true;
            }
            assert(found);
        }
    },
    // Test 3: Throws on degree < 3
    []() -> void {
        std::vector<int> bits = {1, 0};
        pcp::SimplePCP orig_pcp(bits);
        bool thrown = false;
        try {
            core::reduce_degree(orig_pcp, 2);
        } catch (const std::invalid_argument&) {
            thrown = true;
        }
        assert(thrown);
    }
    ,
    // Test 4: Large complete graph, degree 4
    []() -> void {
        const int N = 10;
        std::vector<int> bits(N, 1);
        pcp::SimplePCP orig_pcp(bits);
        // Complete graph: every node connected to every other
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                orig_pcp.add_constraint(i, j, constraint::BinaryNOTEQUAL);
            }
        }
        int degree = 4;
        auto reduced = core::reduce_degree(orig_pcp, degree);
        // Check bits are preserved
        for (int i = 0; i < reduced.get_size(); ++i) {
            assert(reduced.get_variable(i) == true);
        }
        // Check cycle constraints for each original node
        int offset = 0;
        for (int i = 0; i < N; ++i) {
            int sz = orig_pcp.get_constraints(i).size();
            for (int j = 0; j < sz; ++j) {
                int curr = offset + j;
                int next = offset + (j + 1) % sz;
                bool found = false;
                for (const auto &[adj, c] : reduced.get_constraints(curr)) {
                    if (adj == next && c == constraint::BinaryEQUAL) found = true;
                }
                assert(found);
            }
            offset += sz;
        }
        // Check original constraints mapped
        offset = 0;
        for (int i = 0; i < N; ++i) {
            auto orig_constraints = orig_pcp.get_constraints(i);
            for (size_t j = 0; j < orig_constraints.size(); ++j) {
                int adj = orig_constraints[j].first;
                constraint::BinaryConstraint c = orig_constraints[j].second;
                int constraint_pos = orig_pcp.get_constraints_indices(i)[j].second;
                int adj_new_index = 0;
                for (int k = 0; k < adj; ++k) adj_new_index += orig_pcp.get_constraints(k).size();
                adj_new_index += constraint_pos;
                int curr = offset + j;
                bool found = false;
                for (const auto &[a, cc] : reduced.get_constraints(curr)) {
                    if (a == adj_new_index && cc == c) found = true;
                }
                assert(found);
            }
            offset += orig_constraints.size();
        }
    },
    // Test 5: Large star graph, degree 4
    []() -> void {
        const int N = 20;
        std::vector<int> bits(N, 0);
        pcp::SimplePCP orig_pcp(bits);
        for (int i = 1; i < N; ++i) {
            orig_pcp.add_constraint(0, i, constraint::BinaryNOTEQUAL);
        }
        int degree = 4;
        auto reduced = core::reduce_degree(orig_pcp, degree);
        // Check bits are preserved
        for (int i = 0; i < reduced.get_size(); ++i) {
            assert(reduced.get_variable(i) == false);
        }
        // Check cycle constraints for center
        int center_sz = orig_pcp.get_constraints(0).size();
        for (int i = 0; i < center_sz; ++i) {
            int next = (i + 1) % center_sz;
            bool found = false;
            for (const auto &[adj, c] : reduced.get_constraints(i)) {
                if (adj == next && c == constraint::BinaryEQUAL) found = true;
            }
            assert(found);
        }
        // Check original constraints mapped for leaves
        int offset = center_sz;
        for (int i = 1; i < N; ++i) {
            bool found = false;
            for (const auto &[adj, c] : reduced.get_constraints(offset + (i - 1))) {
                if (adj < center_sz && c == constraint::BinaryNOTEQUAL) found = true;
            }
            assert(found);
        }
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