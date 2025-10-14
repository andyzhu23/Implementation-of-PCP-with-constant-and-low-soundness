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

// Test 1: linear chain 0-1-2-3
void test_case_1() {
    std::vector<Color> values { Color::RED, Color::GREEN, Color::BLUE, Color::RED };
    std::vector<csp::Constraint<Color>> constraints {
        {0, 1, not_equal},
        {1, 2, not_equal},
        {2, 3, not_equal},
    };
    csp::CSP<2, Color> p(4, values, constraints);

    // radius 0: should only include the source node
    auto r0 = core::get_radius_nodes(p, 1, 0);
    assert(r0.size() == 1);
    assert(r0.at(1) == p.get_value(1));

    // radius 1: include neighbors
    auto r1 = core::get_radius_nodes(p, 1, 1);
    assert(r1.size() == 3); // nodes 0,1,2
    assert(r1.at(0) == p.get_value(0));
    assert(r1.at(2) == p.get_value(2));

    // radius 2: include node 3 as well
    auto r2 = core::get_radius_nodes(p, 1, 2);
    assert(r2.size() == 4);
}

// Test 2: cycle 0-1-2-0 and branch 1-3
void test_case_2() {
    std::vector<Color> values { Color::RED, Color::GREEN, Color::BLUE, Color::RED };
    std::vector<csp::Constraint<Color>> constraints {
        {0, 1, not_equal},
        {1, 2, not_equal},
        {2, 0, not_equal},
        {1, 3, not_equal},
    };
    csp::CSP<2, Color> p(4, values, constraints);

    // From node 0, radius 1 should include 0,1,2 (cycle neighbors)
    auto r1 = core::get_radius_nodes(p, 0, 1);
    assert(r1.size() == 3);
    assert(r1.find(3) == r1.end());

    // From node 3, radius 2 should reach the whole component
    auto r2_from3 = core::get_radius_nodes(p, 3, 2);
    assert(r2_from3.size() == 4);
    // check that values are preserved
    for (auto &pr : r2_from3) {
        assert(pr.second == p.get_value(pr.first));
    }
}

// Test 3: single isolated node
void test_case_3() {
    std::vector<Color> values { Color::RED };
    std::vector<csp::Constraint<Color>> constraints { };
    csp::CSP<2, Color> p(1, values, constraints);
    auto r = core::get_radius_nodes(p, 0, 5);
    assert(r.size() == 1);
    assert(r.at(0) == Color::RED);
}

int main() {
    test_case_1();
    test_case_2();
    test_case_3();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}