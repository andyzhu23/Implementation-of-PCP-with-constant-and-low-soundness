// TODO rewrite this whole thing

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "pcpp/Tester.hpp"
#include "three_color/ThreeColor.hpp"
#include "pcp/BitPCP.hpp"
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
            case constraint::BitConstraint::FIRST_BIT_NOTEQUAL:
                if (bitpcp.get_variable(u)[0] == bitpcp.get_variable(v)[0]) return false;
                break;
            case constraint::BitConstraint::SECOND_BIT_NOTEQUAL:
                if (bitpcp.get_variable(u)[1] == bitpcp.get_variable(v)[1]) return false;
                break;
            case constraint::BitConstraint::THIRD_BIT_NOTEQUAL:
                if (bitpcp.get_variable(u)[2] == bitpcp.get_variable(v)[2]) return false;
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
        pcpp::Tester tester(three_color::Color::RED, three_color::Color::GREEN);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is satisfiable
        assert(checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be satisfiable for different colors");
    },
    // Test 2: Simple case with two different colors
    []() -> void {
        pcpp::Tester tester(three_color::Color::RED, three_color::Color::BLUE);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is satisfiable
        assert(checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be satisfiable for different colors");
    },
    // Test 3: Simple case with two different colors
    []() -> void {
        pcpp::Tester tester(three_color::Color::GREEN, three_color::Color::BLUE);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is satisfiable
        assert(checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be satisfiable for different colors");
    },
    // Test 4: Simple case with two same colors
    []() -> void {
        pcpp::Tester tester(three_color::Color::RED, three_color::Color::RED);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is not satisfiable
        assert(!checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be unsatisfiable for same colors");
    },
    // Test 5: Simple case with two same colors
    []() -> void {
        pcpp::Tester tester(three_color::Color::GREEN, three_color::Color::GREEN);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is not satisfiable
        assert(!checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be unsatisfiable for same colors");
    },
    // Test 6: Simple case with two same colors
    []() -> void {
        pcpp::Tester tester(three_color::Color::BLUE, three_color::Color::BLUE);
        pcp::BitPCP bitpcp = tester.buildBitPCP();
        // Check that the BitPCP is not satisfiable
        assert(!checkBitPCPCompleteness(bitpcp) && "Expected BitPCP to be unsatisfiable for same colors");
    },
    // Test 7: Test BitPCP completeness with a satisfiable BitPCP
    []() -> void {
        // Create a simple satisfiable BitPCP: two variables with NOTEQUAL constraint
        pcp::BitPCP bitpcp(2);
        bitpcp.set_variable(0, pcp::BitDomain(0b111)); // variable 0 = 111
        bitpcp.set_variable(1, pcp::BitDomain(0b000)); // variable 1 = 000
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::NOTEQUAL);
        pcpp::Tester tester(bitpcp);
        // Check completeness of the BitPCP
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(is_complete && "Expected BitPCP to be complete (satisfiable)");
    },
    // Test 8: Test BitPCP completeness with a satisfiable BitPCP
    []() -> void {
        // Create a BitPCP with EQUAL constraint that is satisfied
        pcp::BitPCP bitpcp(2);
        bitpcp.set_variable(0, pcp::BitDomain(0b101)); // variable 0 = 101
        bitpcp.set_variable(1, pcp::BitDomain(0b101)); // variable 1 = 101
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        pcpp::Tester tester(bitpcp);
        // Check completeness of the BitPCP
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(is_complete && "Expected BitPCP to be complete (satisfiable)");
    },
    // Test 9: Test BitPCP completeness with a satisfiable BitPCP with bit-specific constraints
    []() -> void {
        // Create a BitPCP with FIRST_BIT_EQUAL constraint that is satisfied
        pcp::BitPCP bitpcp(3);
        bitpcp.set_variable(0, pcp::BitDomain(0b001)); // variable 0 = 001
        bitpcp.set_variable(1, pcp::BitDomain(0b011)); // variable 1 = 011
        bitpcp.set_variable(2, pcp::BitDomain(0b101)); // variable 2 = 101
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::FIRST_BIT_EQUAL); // both have bit 0 = 1
        bitpcp.add_constraint(0, 2, constraint::BitConstraint::FIRST_BIT_EQUAL); // both have bit 0 = 1
        // Check completeness of the BitPCP
        pcpp::Tester tester(bitpcp);
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(is_complete && "Expected BitPCP to be complete (satisfiable)");
    },
    // Test 10: Test BitPCP completeness with an unsatisfiable BitPCP
    []() -> void {
        // Create an unsatisfiable BitPCP: two equal variables with NOTEQUAL constraint
        pcp::BitPCP bitpcp(2);
        bitpcp.set_variable(0, pcp::BitDomain(0b110)); // variable 0 = 000
        bitpcp.set_variable(1, pcp::BitDomain(0b110)); // variable 1 = 111
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::NOTEQUAL);
        // Check completeness of the BitPCP
        pcpp::Tester tester(bitpcp);
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(!is_complete && "Expected BitPCP to be incomplete (unsatisfiable)");
    },
    // Test 11: Test BitPCP completeness with an unsatisfiable BitPCP
    []() -> void {
        // Create an unsatisfiable BitPCP: two different variables with EQUAL constraint
        pcp::BitPCP bitpcp(2);
        bitpcp.set_variable(0, pcp::BitDomain(0b111)); // variable 0 = 111
        bitpcp.set_variable(1, pcp::BitDomain(0b000)); // variable 1 = 000
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // Check completeness of the BitPCP
        pcpp::Tester tester(bitpcp);
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(!is_complete && "Expected BitPCP to be incomplete (unsatisfiable)");
    },
    // Test 12: Test BitPCP completeness with an unsatisfiable BitPCP with bit-specific constraints
    []() -> void {
        // Create an unsatisfiable BitPCP: FIRST_BIT_EQUAL constraint violated
        pcp::BitPCP bitpcp(2);
        bitpcp.set_variable(0, pcp::BitDomain(0b001)); // variable 0 = 001 (first bit = 1)
        bitpcp.set_variable(1, pcp::BitDomain(0b010)); // variable 1 = 010 (first bit = 0)
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
        // Check completeness of the BitPCP
        pcpp::Tester tester(bitpcp);
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(!is_complete && "Expected BitPCP to be incomplete (unsatisfiable)");
    },
    // Test 13: Test BitPCP completeness with an unsatisfiable BitPCP with bit-specific constraints
    []() -> void {
        // Create an unsatisfiable BitPCP: FIRST_BIT_EQUAL constraint violated
        pcp::BitPCP bitpcp(3);
        bitpcp.set_variable(0, pcp::BitDomain(0b001)); // variable 0 = 001 (first bit = 1)
        bitpcp.set_variable(1, pcp::BitDomain(0b010)); // variable 1 = 010 (first bit = 0)
        bitpcp.set_variable(2, pcp::BitDomain(0b111)); // variable 2 = 111 
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
        bitpcp.add_constraint(0, 2, constraint::BitConstraint::THIRD_BIT_NOTEQUAL);
        // Check completeness of the BitPCP
        pcpp::Tester tester(bitpcp);
        bool is_complete = checkBitPCPCompleteness(tester.buildBitPCP());
        assert(!is_complete && "Expected BitPCP to be incomplete (unsatisfiable)");
    },

};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
