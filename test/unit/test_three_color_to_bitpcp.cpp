#include <functional>
#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>

#include "three_color/ThreeColor.hpp"
#include "pcp/BitPCP.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: verify color_to_bits mapping
    []() -> void {
        using three_color::Color;
        auto b_red = three_color::color_to_bits(Color::RED);
        auto b_green = three_color::color_to_bits(Color::GREEN);
        auto b_blue = three_color::color_to_bits(Color::BLUE);
        // RED -> 00
        assert(b_red[0] == 0);
        assert(b_red[1] == 0);
        // GREEN -> 01
        assert(b_green[0] == 0);
        assert(b_green[1] == 1);
        // BLUE -> 10
        assert(b_blue[0] == 1);
        assert(b_blue[1] == 0);
    },
    // Test 2: two nodes one edge converts to a non-empty BitPCP
    []() -> void {
        using three_color::ThreeColor;
        using three_color::Color;
        using three_color::Edge;
        // Create 3-node triangle with distinct colors
        std::vector<Color> colors = { Color::RED, Color::GREEN };
        std::vector<Edge> edges = { {0,1} };
        ThreeColor tc(colors, edges);
        auto pcp = tc.to_BitPCP();
        // The resulting BitPCP should satisfy the constraints
        for (pcp::Variable var = 0; var < pcp.get_size(); ++var) {
            for (const auto& [other_var, constraint] : pcp.get_constraints(var)) {
                switch (constraint) {
                    case constraint::BitConstraint::NOTEQUAL:
                        assert(pcp.get_variable(var) != pcp.get_variable(other_var));
                        break;
                    case constraint::BitConstraint::EQUAL:
                        assert(pcp.get_variable(var) == pcp.get_variable(other_var));
                        break;
                    case constraint::BitConstraint::ANY:
                        break;
                }
            }
        }
    },
    // Test 3: invalid coloring two nodes one edge converts to a non-empty BitPCP
    []() -> void {
        using three_color::ThreeColor;
        using three_color::Color;
        using three_color::Edge;
        // Create 3-node triangle with distinct colors
        std::vector<Color> colors = { Color::RED, Color::RED };
        std::vector<Edge> edges = { {0,1} };
        ThreeColor tc(colors, edges);
        auto pcp = tc.to_BitPCP();
        int invalid_count = 0;
        // The resulting BitPCP should not satisfy the constraints
        for (pcp::Variable var = 0; var < pcp.get_size(); ++var) {
            for (const auto& [other_var, constraint] : pcp.get_constraints(var)) {
                switch (constraint) {
                    case constraint::BitConstraint::NOTEQUAL:
                        if (pcp.get_variable(var) == pcp.get_variable(other_var)) {
                            invalid_count++;
                        }
                        break;
                    case constraint::BitConstraint::EQUAL:
                        if (pcp.get_variable(var) != pcp.get_variable(other_var)) {
                            invalid_count++;
                        }
                        break;
                    case constraint::BitConstraint::ANY:
                        break;
                }
            }
        }
        assert(invalid_count > 0);
    },
    // Test 4: small triangle graph converts to a non-empty BitPCP
    []() -> void {
        using three_color::ThreeColor;
        using three_color::Color;
        using three_color::Edge;
        // Create 3-node triangle with distinct colors
        std::vector<Color> colors = { Color::RED, Color::GREEN, Color::BLUE };
        std::vector<Edge> edges = { {0,1}, {1,2}, {2,0} };
        ThreeColor tc(colors, edges);
        auto pcp = tc.to_BitPCP();
        // The resulting BitPCP should satisfy the constraints
        for (pcp::Variable var = 0; var < pcp.get_size(); ++var) {
            for (const auto& [other_var, constraint] : pcp.get_constraints(var)) {
                switch (constraint) {
                    case constraint::BitConstraint::NOTEQUAL:
                        assert(pcp.get_variable(var) != pcp.get_variable(other_var));
                        break;
                    case constraint::BitConstraint::EQUAL:
                        assert(pcp.get_variable(var) == pcp.get_variable(other_var));
                        break;
                    case constraint::BitConstraint::ANY:
                        break;
                }
            }
        }
    },
    // Test 5: an invalid coloring small triangle graph converts to a non-empty BitPCP
    []() -> void {
        using three_color::ThreeColor;
        using three_color::Color;
        using three_color::Edge;
        // Create 3-node triangle with distinct colors
        std::vector<Color> colors = { Color::RED, Color::RED, Color::BLUE };
        std::vector<Edge> edges = { {0,1}, {1,2}, {2,0} };
        ThreeColor tc(colors, edges);
        auto pcp = tc.to_BitPCP();
        int invalid_count = 0;
        // The resulting BitPCP not should satisfy the constraints
        for (pcp::Variable var = 0; var < pcp.get_size(); ++var) {
            for (const auto& [other_var, constraint] : pcp.get_constraints(var)) {
                switch (constraint) {
                    case constraint::BitConstraint::NOTEQUAL:
                        if (pcp.get_variable(var) == pcp.get_variable(other_var)) {
                            invalid_count++;
                        }
                        break;
                    case constraint::BitConstraint::EQUAL:
                        if (pcp.get_variable(var) != pcp.get_variable(other_var)) {
                            invalid_count++;
                        }
                        break;
                    case constraint::BitConstraint::ANY:
                        break;
                }
            }
        }
        assert(invalid_count > 0);
    }
};


int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        // Run each test case multiple times to catch flakiness
        for (int trial = 0; trial < 3; ++trial) {
            test_cases[i]();
        }
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}