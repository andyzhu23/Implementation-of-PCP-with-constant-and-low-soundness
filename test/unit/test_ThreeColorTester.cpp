// TODO rewrite this whole thing

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "pcpp/ThreeColorTester.hpp"
#include "three_color/ThreeColor.hpp"
#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"
#include "constraint/BitConstraint.hpp"

// Helper function to check completeness of a BitPCP
// Returns true if all constraints are satisfied, false otherwise
bool checkBitPCPCompleteness(const pcp::BitPCP &bitpcp) {
    for (const auto &[u, v, c] : bitpcp.get_constraints_list()) {
        switch (c) {
            case constraint::BitConstraint::NOTEQUAL:
                if (bitpcp.get_variable(u) == bitpcp.get_variable(v)) return false;
                break;
            case constraint::BitConstraint::EQUAL:
                if (bitpcp.get_variable(u) != bitpcp.get_variable(v)) return false;
                break;
            case constraint::BitConstraint::FIRST_BIT_EQUAL:
                if (bitpcp.get_variable(u)[0] != bitpcp.get_variable(v)[0]) return false;
                break;
            case constraint::BitConstraint::SECOND_BIT_EQUAL:
                if (bitpcp.get_variable(u)[1] != bitpcp.get_variable(v)[1]) return false;
                break;
            case constraint::BitConstraint::THIRD_BIT_EQUAL:
                if (bitpcp.get_variable(u)[2] != bitpcp.get_variable(v)[2]) return false;
                break;
            default:
                break;
        }
    }
    return true;
}

std::vector<std::function<void()>> test_cases = {
    // Test 1: Simple case with two different colors
    []() -> void {
        pcpp::ThreeColorTester tester(three_color::Color::RED, three_color::Color::GREEN);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is satisfiable
        assert(checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be satisfiable for different colors");
    },
    // Test 2: Simple case with two different colors
    []() -> void {
        pcpp::ThreeColorTester tester(three_color::Color::RED, three_color::Color::BLUE);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is satisfiable
        assert(checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be satisfiable for different colors");
    },
    // Test 3: Simple case with two different colors
    []() -> void {
        pcpp::ThreeColorTester tester(three_color::Color::GREEN, three_color::Color::BLUE);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is satisfiable
        assert(checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be satisfiable for different colors");
    },
    // Test 4: Simple case with two same colors
    []() -> void {
        pcpp::ThreeColorTester tester(three_color::Color::RED, three_color::Color::RED);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is not satisfiable
        assert(!checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be unsatisfiable for same colors");
    },
    // Test 5: Simple case with two same colors
    []() -> void {
        pcpp::ThreeColorTester tester(three_color::Color::GREEN, three_color::Color::GREEN);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is not satisfiable
        assert(!checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be unsatisfiable for same colors");
    },
    // Test 6: Simple case with two same colors
    []() -> void {
        pcpp::ThreeColorTester tester(three_color::Color::BLUE, three_color::Color::BLUE);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is not satisfiable
        assert(!checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be unsatisfiable for same colors");
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
