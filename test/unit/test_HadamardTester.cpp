// TODO rewrite this whole thing

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "pcpp/HadamardPCPP/HadamardTester.hpp"
#include "three_color/ThreeColor.hpp"
#include "pcp/BinaryCSP.hpp"
#include "pcp/BinaryDomain.hpp"
#include "constraint/BinaryConstraint.hpp"

// Helper function to check completeness of a BinaryCSP
// Returns true if all constraints are satisfied, false otherwise
bool checkBinaryCSPCompleteness(const pcp::BinaryCSP &BinaryCSP) {
    for (const auto &[u, v, c] : BinaryCSP.get_constraints_list()) {
        switch (c) {
            case constraint::BinaryConstraint::NOTEQUAL:
                if (BinaryCSP.get_variable(u) == BinaryCSP.get_variable(v)) return false;
                break;
            case constraint::BinaryConstraint::EQUAL:
                if (BinaryCSP.get_variable(u) != BinaryCSP.get_variable(v)) return false;
                break;
            case constraint::BinaryConstraint::FIRST_BIT_EQUAL:
                if (BinaryCSP.get_variable(u)[0] != BinaryCSP.get_variable(v)[0]) return false;
                break;
            case constraint::BinaryConstraint::SECOND_BIT_EQUAL:
                if (BinaryCSP.get_variable(u)[1] != BinaryCSP.get_variable(v)[1]) return false;
                break;
            case constraint::BinaryConstraint::THIRD_BIT_EQUAL:
                if (BinaryCSP.get_variable(u)[2] != BinaryCSP.get_variable(v)[2]) return false;
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
        pcpp::HadamardTester tester(three_color::Color::RED, three_color::Color::GREEN);
        pcp::BinaryCSP BinaryCSP = tester.buildBinaryCSP();
        // Check that the BinaryCSP is satisfiable
        assert(checkBinaryCSPCompleteness(BinaryCSP) && "Expected BinaryCSP to be satisfiable for different colors");
    },
    // Test 2: Simple case with two different colors
    []() -> void {
        pcpp::HadamardTester tester(three_color::Color::GREEN, three_color::Color::BLUE);
        pcp::BinaryCSP BinaryCSP = tester.buildBinaryCSP();
        // Check that the BinaryCSP is satisfiable
        assert(checkBinaryCSPCompleteness(BinaryCSP) && "Expected BinaryCSP to be satisfiable for different colors");
    },
    // Test 3: Simple case with two different colors
    []() -> void {
        pcpp::HadamardTester tester(three_color::Color::RED, three_color::Color::BLUE);
        pcp::BinaryCSP BinaryCSP = tester.buildBinaryCSP();
        // Check that the BinaryCSP is satisfiable
        assert(checkBinaryCSPCompleteness(BinaryCSP) && "Expected BinaryCSP to be satisfiable for different colors");
    },
    // Test 4: Simple case with two same colors
    []() -> void {
        pcpp::HadamardTester tester(three_color::Color::RED, three_color::Color::RED);
        pcp::BinaryCSP BinaryCSP = tester.buildBinaryCSP();
        // Check that the BinaryCSP is not satisfiable
        assert(!checkBinaryCSPCompleteness(BinaryCSP) && "Expected BinaryCSP to be unsatisfiable for same colors");
    },
    // Test 5: Simple case with two same colors
    []() -> void {
        pcpp::HadamardTester tester(three_color::Color::GREEN, three_color::Color::GREEN);
        pcp::BinaryCSP BinaryCSP = tester.buildBinaryCSP();
        // Check that the BinaryCSP is not satisfiable
        assert(!checkBinaryCSPCompleteness(BinaryCSP) && "Expected BinaryCSP to be unsatisfiable for same colors");
    },
    // Test 6: Simple case with two same colors
    []() -> void {
        pcpp::HadamardTester tester(three_color::Color::BLUE, three_color::Color::BLUE);
        pcp::BinaryCSP BinaryCSP = tester.buildBinaryCSP();
        // Check that the BinaryCSP is not satisfiable
        assert(!checkBinaryCSPCompleteness(BinaryCSP) && "Expected BinaryCSP to be unsatisfiable for same colors");
    },
    // Test 7: Test BinaryCSP completeness with a satisfiable BinaryCSP
    []() -> void {
        // Create a simple satisfiable BinaryCSP: two variables with NOTEQUAL constraint
        pcp::BinaryCSP BinaryCSP(2);
        BinaryCSP.set_variable(0, pcp::BinaryDomain(0b001)); // variable 0 = 001
        BinaryCSP.set_variable(1, pcp::BinaryDomain(0b010)); // variable 1 = 010
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::NOTEQUAL);
        // Check completeness of the BinaryCSP
        bool is_complete = checkBinaryCSPCompleteness(BinaryCSP);
        assert(is_complete && "Expected BinaryCSP to be complete (satisfiable)");
    },
    // Test 8: Test BinaryCSP completeness with a satisfiable BinaryCSP
    []() -> void {
        // Create a BinaryCSP with EQUAL constraint that is satisfied
        pcp::BinaryCSP BinaryCSP(2);
        BinaryCSP.set_variable(0, pcp::BinaryDomain(0b101)); // variable 0 = 101
        BinaryCSP.set_variable(1, pcp::BinaryDomain(0b101)); // variable 1 = 101
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // Check completeness of the BinaryCSP
        bool is_complete = checkBinaryCSPCompleteness(BinaryCSP);
        assert(is_complete && "Expected BinaryCSP to be complete (satisfiable)");
    },
    // Test 9: Test BinaryCSP completeness with a satisfiable BinaryCSP with bit-specific constraints
    []() -> void {
        // Create a BinaryCSP with FIRST_BIT_EQUAL constraint that is satisfied
        pcp::BinaryCSP BinaryCSP(3);
        BinaryCSP.set_variable(0, pcp::BinaryDomain(0b001)); // variable 0 = 001
        BinaryCSP.set_variable(1, pcp::BinaryDomain(0b011)); // variable 1 = 011
        BinaryCSP.set_variable(2, pcp::BinaryDomain(0b101)); // variable 2 = 101
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::FIRST_BIT_EQUAL); // both have bit 0 = 1
        BinaryCSP.add_constraint(0, 2, constraint::BinaryConstraint::FIRST_BIT_EQUAL); // both have bit 0 = 1
        // Check completeness of the BinaryCSP
        bool is_complete = checkBinaryCSPCompleteness(BinaryCSP);
        assert(is_complete && "Expected BinaryCSP to be complete (satisfiable)");
    },
    // Test 10: Test BinaryCSP completeness with an unsatisfiable BinaryCSP
    []() -> void {
        // Create an unsatisfiable BinaryCSP: two equal variables with NOTEQUAL constraint
        pcp::BinaryCSP BinaryCSP(2);
        BinaryCSP.set_variable(0, pcp::BinaryDomain(0b110)); // variable 0 = 110
        BinaryCSP.set_variable(1, pcp::BinaryDomain(0b110)); // variable 1 = 110
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::NOTEQUAL);
        // Check completeness of the BinaryCSP
        bool is_complete = checkBinaryCSPCompleteness(BinaryCSP);
        assert(!is_complete && "Expected BinaryCSP to be incomplete (unsatisfiable)");
    },
    // Test 11: Test BinaryCSP completeness with an unsatisfiable BinaryCSP
    []() -> void {
        // Create an unsatisfiable BinaryCSP: two different variables with EQUAL constraint
        pcp::BinaryCSP BinaryCSP(2);
        BinaryCSP.set_variable(0, pcp::BinaryDomain(0b111)); // variable 0 = 111
        BinaryCSP.set_variable(1, pcp::BinaryDomain(0b000)); // variable 1 = 000
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // Check completeness of the BinaryCSP
        bool is_complete = checkBinaryCSPCompleteness(BinaryCSP);
        assert(!is_complete && "Expected BinaryCSP to be incomplete (unsatisfiable)");
    },
    // Test 12: Test BinaryCSP completeness with an unsatisfiable BinaryCSP with bit-specific constraints
    []() -> void {
        // Create an unsatisfiable BinaryCSP: FIRST_BIT_EQUAL constraint violated
        pcp::BinaryCSP BinaryCSP(2);
        BinaryCSP.set_variable(0, pcp::BinaryDomain(0b001)); // variable 0 = 001 (first bit = 1)
        BinaryCSP.set_variable(1, pcp::BinaryDomain(0b010)); // variable 1 = 010 (first bit = 0)
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::FIRST_BIT_EQUAL);
        // Check completeness of the BinaryCSP
        bool is_complete = checkBinaryCSPCompleteness(BinaryCSP);
        assert(!is_complete && "Expected BinaryCSP to be incomplete (unsatisfiable)");
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
