#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: 5-node cycle, alternating constraints
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1};
        pcp::BitPCP pcp(bits);
        for (int i = 0; i < 5; ++i) {
            pcp.add_constraint(i, (i + 1) % 5, constraint::BitConstraint::EQUAL);
        }
        // Radius 1 from node 0
        auto neighbors1 = pcp.get_neighbors(0, 1);
        assert(std::find(neighbors1.begin(), neighbors1.end(), 0) != neighbors1.end());
        assert(std::find(neighbors1.begin(), neighbors1.end(), 1) != neighbors1.end());
        assert(std::find(neighbors1.begin(), neighbors1.end(), 4) != neighbors1.end());
        // Radius 2 from node 2
        auto neighbors2 = pcp.get_neighbors(2, 2);
        for (int i = 0; i < 5; ++i) {
            assert(std::find(neighbors2.begin(), neighbors2.end(), i) != neighbors2.end());
        }
        assert(neighbors2.size() == 5);
    },
    // Test 2: Star graph, center 0, leaves 1-4
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1};
        pcp::BitPCP pcp(bits);
        for (int i = 1; i < 5; ++i) {
            pcp.add_constraint(0, i, constraint::BitConstraint::EQUAL);
        }
        // Center node, radius 1
        auto neighbors = pcp.get_neighbors(0, 1);
        for (int i = 0; i < 5; ++i) {
            assert(std::find(neighbors.begin(), neighbors.end(), i) != neighbors.end());
        }
        assert(neighbors.size() == 5);
        // Leaf node, radius 1
        auto leaf_neighbors = pcp.get_neighbors(2, 1);
        assert(std::find(leaf_neighbors.begin(), leaf_neighbors.end(), 0) != leaf_neighbors.end());
        assert(std::find(leaf_neighbors.begin(), leaf_neighbors.end(), 2) != leaf_neighbors.end());
        assert(leaf_neighbors.size() == 2);
    }, 
    // Test 3: 6-node chain, alternating constraints
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0, 1, 0};
        pcp::BitPCP pcp(bits);
        for (int i = 0; i < 5; ++i) {
            pcp.add_constraint(i, i+1, constraint::BitConstraint::EQUAL);
        }
        // Middle node, radius 2
        auto neighbors = pcp.get_neighbors(3, 2);
        // Should include nodes 1,2,3,4,5
        for (int i = 1; i <= 5; ++i) {
            assert(std::find(neighbors.begin(), neighbors.end(), i) != neighbors.end());
        }
        assert(neighbors.size() == 5);
    }, 
    // Test 4: 4-node complete graph
    []() -> void {
        std::vector<pcp::BitDomain> bits = {1, 0, 1, 0};
        pcp::BitPCP pcp(bits);
        for (int i = 0; i < 4; ++i) {
            for (int j = i+1; j < 4; ++j) {
                pcp.add_constraint(i, j, constraint::BitConstraint::EQUAL);
            }
        }
        // Any node, radius 1
        auto neighbors = pcp.get_neighbors(2, 1);
        for (int i = 0; i < 4; ++i) {
            assert(std::find(neighbors.begin(), neighbors.end(), i) != neighbors.end());
        }
        assert(neighbors.size() == 4);
        // Edge case: radius 0 (should only return itself)
        auto self = pcp.get_neighbors(3, 0);
        assert(self.size() == 1);
        assert(self[0] == 3);
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