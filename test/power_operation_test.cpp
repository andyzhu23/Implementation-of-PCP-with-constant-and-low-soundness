#include <functional>
#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_map>

#include "core.hpp"
#include "csp.hpp"

enum class Color {
    RED,
    GREEN,
    BLUE,
};

std::function<bool(Color, Color)> not_equal = [](Color a, Color b) -> bool { return a != b; };

// Test 1: simple CSP with not-equal constraints
void test_case_1() {
    // build a 4-node CSP with simple not-equal constraints
    std::vector<Color> values { Color::RED, Color::GREEN, Color::BLUE, Color::RED };
    std::vector<csp::Constraint<Color>> constraints {
        {0, 1, not_equal},
        {1, 2, not_equal},
        {1, 3, not_equal},
    };

    csp::CSP<2, Color> p(4, values, constraints);

    // radius = 1: each node's value in the powered CSP should include itself
    auto powered = core::power_operation(p, 1);
    assert(powered.get_num_node() == 4);
    for (int i = 0; i < 4; ++i) {
        const auto &map_i = powered.get_value(i);
        // each neighborhood must contain the original node
        assert(map_i.find(i) != map_i.end());
        assert(map_i.at(i) == p.get_value(i));
    }

    // verify transformed constraint between 0 and 1 exists and matches original relation
    const auto &constraints_view = powered.get_constraints();
    bool found01 = false;
    for (const auto &[node, constraint] : constraints_view[0]) {
        if (node == 1) {
            found01 = true;
            bool transformed = constraint(powered.get_value(0), powered.get_value(1));
            bool original = not_equal(p.get_value(0), p.get_value(1));
            assert(transformed == original);
        }
    }
    assert(found01);
}


// Test 2: power operation with radius = 2
void test_case_2() {
    // same CSP as above; use radius = 2 to include nodes at distance 2
    std::vector<Color> values { Color::RED, Color::GREEN, Color::BLUE, Color::RED };
    std::vector<csp::Constraint<Color>> constraints {
        {0, 1, not_equal},
        {1, 2, not_equal},
        {1, 3, not_equal},
    };

    csp::CSP<2, Color> p(4, values, constraints);
    auto powered2 = core::power_operation(p, 2);

    // node 0 should see nodes 0,1 and 2 within radius 2 (0->1->2)
    const auto &map0 = powered2.get_value(0);
    assert(map0.find(0) != map0.end());
    assert(map0.find(1) != map0.end());
    assert(map0.find(2) != map0.end());
    // values should match original CSP values
    assert(map0.at(2) == p.get_value(2));
}


// Test 3: more complex CSP with 6 nodes, more constraints, and radius = 3
void test_case_3() {
    // 6 nodes, with a cycle and cross constraints
    std::vector<Color> values { Color::RED, Color::GREEN, Color::BLUE, Color::RED, Color::GREEN, Color::BLUE };
    std::vector<csp::Constraint<Color>> constraints {
        {0, 1, not_equal},
        {1, 2, not_equal},
        {2, 3, not_equal},
        {3, 4, not_equal},
        {4, 5, not_equal},
        {5, 0, not_equal}, // cycle
        {0, 3, not_equal}, // cross constraint
        {2, 5, not_equal}  // cross constraint
    };

    csp::CSP<2, Color> p(6, values, constraints);
    auto powered3 = core::power_operation(p, 3);


    // For each node, check its neighborhood within radius 3 and values
    // Precomputed expected neighborhoods for each node within radius 3
    std::vector<std::vector<int>> expected_neighbors = {
        {0,1,2,3},    // node 0
        {0,1,2,3,4},  // node 1
        {0,1,2,3,4,5},// node 2
        {0,1,2,3,4,5},// node 3
        {1,2,3,4,5},  // node 4
        {0,2,3,4,5}   // node 5
    };
    for (int i = 0; i < 6; ++i) {
        const auto &map_i = powered3.get_value(i);
        // Check all expected neighbors are present
        for (int n : expected_neighbors[i]) {
            assert(map_i.find(n) != map_i.end());
            // Value matches original CSP
            assert(map_i.at(n) == p.get_value(n));
        }
    }

    // Check that the powered CSP preserves the not-equal constraint for a cross edge (0,3)
    const auto &constraints_view = powered3.get_constraints();
    bool found03 = false;
    for (const auto &[node, constraint] : constraints_view[0]) {
        if (node == 3) {
            found03 = true;
            bool transformed = constraint(powered3.get_value(0), powered3.get_value(3));
            bool original = not_equal(p.get_value(0), p.get_value(3));
            assert(transformed == original);
        }
    }
    assert(found03);

    // Check that all nodes in the powered CSP have the correct number of neighbors (at least 1, at most 5)
    for (int i = 0; i < 6; ++i) {
        const auto &map_i = powered3.get_value(i);
        assert(map_i.find(i) != map_i.end());
        // Should not have more than 6 nodes in the neighborhood
        assert(map_i.size() <= 6);
    }
}

int main() {
    test_case_1();
    test_case_2();
    test_case_3();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}