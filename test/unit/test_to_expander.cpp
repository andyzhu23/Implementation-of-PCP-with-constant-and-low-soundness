#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: 5-node cycle, to_expander should add ANY constraints
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1};
        pcp::BitPCP pcp(bits);
        for (int i = 0; i < 5; ++i) {
            pcp.add_constraint(i, (i + 1) % 5, constraint::BitConstraint::EQUAL);
        }
        int expanding_coefficient = 2;
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        // Each node should have at least expanding_coefficient ANY constraints (possibly more if duplicates)
        for (int i = 0; i < 5; ++i) {
            int any_count = 0;
            for (const auto &[adj, constraint] : expander.get_constraints(i)) {
                if (constraint == constraint::BitConstraint::ANY) ++any_count;
            }
            assert(any_count >= expanding_coefficient);
        }
    },
    // Test 2: Star graph, check that new ANY constraints are added
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1};
        pcp::BitPCP pcp(bits);
        for (int i = 1; i < 5; ++i) {
            pcp.add_constraint(0, i, constraint::BitConstraint::NOTEQUAL);
        }
        int expanding_coefficient = 3;
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        for (int i = 0; i < 5; ++i) {
            int any_count = 0;
            for (const auto &[adj, constraint] : expander.get_constraints(i)) {
                if (constraint == constraint::BitConstraint::ANY) ++any_count;
            }
            assert(any_count >= expanding_coefficient);
        }
    },
    // Test 3: Disconnected graph, all nodes should get ANY constraints
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1};
        pcp::BitPCP pcp(bits);
        int expanding_coefficient = 2;
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        for (int i = 0; i < 3; ++i) {
            int any_count = 0;
            for (const auto &[adj, constraint] : expander.get_constraints(i)) {
                if (constraint == constraint::BitConstraint::ANY) ++any_count;
            }
            assert(any_count >= expanding_coefficient);
        }
    },
    // Test 4: Single node, should not add self-loop ANY constraints
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1};
        pcp::BitPCP pcp(bits);
        int expanding_coefficient = 5;
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        // Should not have ANY constraints to itself
        for (const auto &[adj, constraint] : expander.get_constraints(0)) {
            assert(adj != 0 || constraint != constraint::BitConstraint::ANY);
        }
    },
    // Test 5: Extreme case - chain of 100,000 nodes, check max shortest path (diameter)
    []() -> void {
        const int N = 100'000;
        const int expanding_coefficient = 10;
        std::vector<pcp::BitDomain> bits(N, 0);
        pcp::BitPCP pcp(bits);
        // Create a chain
        for (int i = 0; i < N - 1; ++i) {
            pcp.add_constraint(i, i + 1, constraint::BitConstraint::EQUAL);
        }
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        // BFS from node 0 to find the farthest node (diameter)
        std::vector<int> dist(N, -1);
        std::vector<int> q;
        dist[0] = 0;
        q.push_back(0);
        size_t head = 0;
        while (head < q.size()) {
            int u = q[head++];
            for (const auto &[v, constraint] : expander.get_constraints(u)) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push_back(v);
                }
            }
        }
        int max_dist = 0;
        for (int d : dist) {
            if (d > max_dist) max_dist = d;
        }
        // The diameter should be within 20 (with high probability) due to random expansion
        assert(max_dist <= 10);
    }
    ,
    // Test 6: Extreme case - chain of 100,000 nodes, BFS from middle node
    []() -> void {
        const int N = 100'000;
        const int expanding_coefficient = 10;
        std::vector<pcp::BitDomain> bits(N, 0);
        pcp::BitPCP pcp(bits);
        for (int i = 0; i < N - 1; ++i) {
            pcp.add_constraint(i, i + 1, constraint::BitConstraint::EQUAL);
        }
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        int start = N / 2;
        std::vector<int> dist(N, -1);
        std::vector<int> q;
        dist[start] = 0;
        q.push_back(start);
        size_t head = 0;
        while (head < q.size()) {
            int u = q[head++];
            for (const auto &[v, constraint] : expander.get_constraints(u)) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push_back(v);
                }
            }
        }
        int max_dist = 0;
        for (int d : dist) {
            if (d > max_dist) max_dist = d;
        }
        assert(max_dist <= 10);
    }
    ,
    // Test 7: Extreme case - chain of 100,000 nodes, BFS from last node
    []() -> void {
        const int N = 100'000;
        const int expanding_coefficient = 10;
        std::vector<pcp::BitDomain> bits(N, 0);
        pcp::BitPCP pcp(bits);
        for (int i = 0; i < N - 1; ++i) {
            pcp.add_constraint(i, i + 1, constraint::BitConstraint::EQUAL);
        }
        auto &expander = core::to_expander(pcp, expanding_coefficient);
        int start = N - 1;
        std::vector<int> dist(N, -1);
        std::vector<int> q;
        dist[start] = 0;
        q.push_back(start);
        size_t head = 0;
        while (head < q.size()) {
            int u = q[head++];
            for (const auto &[v, constraint] : expander.get_constraints(u)) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push_back(v);
                }
            }
        }
        int max_dist = 0;
        for (int d : dist) {
            if (d > max_dist) max_dist = d;
        }
        assert(max_dist <= 10);
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        // Run each test case multiple times to catch randomness issues
        for (int trial = 0; trial < 3; ++trial) {
            test_cases[i]();
        }
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
