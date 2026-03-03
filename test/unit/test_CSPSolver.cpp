#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "pcpp/PseudoPCPP/CSPSolver.hpp"
#include "pcp/BinaryCSP.hpp"
#include "pcp/BinaryDomain.hpp"
#include "constraint/BinaryConstraint.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: Simple satisfiable BinaryCSP with EQUAL constraints
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "ENCODED_BINARY domain with EQUAL constraint should be satisfiable");
    },
    
    // Test 2: Simple unsatisfiable BinaryCSP - conflicting constraints
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::NOTEQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(!satisfiable && "Conflicting EQUAL and NOTEQUAL constraints should be unsatisfiable");
    },
    
    // Test 3: Triangle graph with satisfiable constraints
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ONE_HOT_COLOR),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ONE_HOT_COLOR),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ONE_HOT_COLOR)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::NOTEQUAL);
        pcp.add_constraint(1, 2, constraint::BinaryConstraint::NOTEQUAL);
        pcp.add_constraint(0, 2, constraint::BinaryConstraint::NOTEQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "3-coloring problem with ONE_HOT_COLOR domain should be satisfiable");
    },
    
    // Test 4: Triangle graph with unsatisfiable constraints (forcing equality in a cycle)
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        pcp.add_constraint(1, 2, constraint::BinaryConstraint::EQUAL);
        pcp.add_constraint(0, 2, constraint::BinaryConstraint::NOTEQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(!satisfiable && "Triangle with EQUAL chain and NOTEQUAL closing should be unsatisfiable");
    },
    
    // Test 5: Bit-specific constraints - FIRST_BIT_EQUAL satisfiable
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::PRODUCT)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::FIRST_BIT_EQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "PRODUCT domain with FIRST_BIT_EQUAL should be satisfiable");
    },
    
    // Test 6: Bit-specific constraints - complex bit constraints
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::SUM),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::SUM),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::SUM)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::FIRST_BIT_EQUAL);
        pcp.add_constraint(1, 2, constraint::BinaryConstraint::SECOND_BIT_EQUAL);
        pcp.add_constraint(0, 2, constraint::BinaryConstraint::THIRD_BIT_EQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "SUM domain with mixed bit constraints should be satisfiable");
    },
    
    // Test 7: Empty BinaryCSP should be satisfiable (but CSPSolver expects at least one variable)
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ANY)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        // No constraints
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "Single variable with no constraints should be satisfiable");
    },
    
    // Test 8: Single variable with no constraints should be satisfiable
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ANY)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "Single variable with ANY domain should be satisfiable");
    },
    
    // Test 9: Chain of EQUAL constraints - satisfiable
    []() -> void {
        const int N = 5;
        std::vector<pcp::BinaryDomain> bits(N, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        pcp::BinaryCSP pcp(std::move(bits));
        for (int i = 0; i < N - 1; ++i) {
            pcp.add_constraint(i, i + 1, constraint::BinaryConstraint::EQUAL);
        }
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "Chain of EQUAL constraints should be satisfiable");
    },
    
    // Test 10: Chain of EQUAL constraints with contradictory closing - unsatisfiable
    []() -> void {
        const int N = 5;
        std::vector<pcp::BinaryDomain> bits(N, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        pcp::BinaryCSP pcp(std::move(bits));
        for (int i = 0; i < N - 1; ++i) {
            pcp.add_constraint(i, i + 1, constraint::BinaryConstraint::EQUAL);
        }
        pcp.add_constraint(0, N - 1, constraint::BinaryConstraint::NOTEQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(!satisfiable && "Chain of EQUAL with contradictory close should be unsatisfiable");
    },
    
    // Test 11: Multiple domain types - mixed constraints
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::SUM)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::FIRST_BIT_EQUAL);
        pcp.add_constraint(1, 2, constraint::BinaryConstraint::SECOND_BIT_EQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "Mixed domain types with bit constraints should be satisfiable");
    },
    
    // Test 12: Complex satisfiable case - star graph with fewer nodes
    []() -> void {
        const int N = 4; // center + 3 leaves (should be satisfiable with ONE_HOT_COLOR)
        std::vector<pcp::BinaryDomain> bits(N, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ONE_HOT_COLOR));
        pcp::BinaryCSP pcp(std::move(bits));
        
        // Center (node 0) connected to all others with NOTEQUAL
        for (int i = 1; i < N; ++i) {
            pcp.add_constraint(0, i, constraint::BinaryConstraint::NOTEQUAL);
        }
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "Star graph with ONE_HOT_COLOR (4 nodes) should be satisfiable");
    },
    
    // Test 13: Complex unsatisfiable case - over-constrained star
    []() -> void {
        const int N = 4; // center + 3 leaves
        std::vector<pcp::BinaryDomain> bits(N, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ONE_HOT_COLOR));
        pcp::BinaryCSP pcp(std::move(bits));
        
        // Center connected to all leaves with NOTEQUAL
        for (int i = 1; i < N; ++i) {
            pcp.add_constraint(0, i, constraint::BinaryConstraint::NOTEQUAL);
        }
        // Force all leaves to be equal to each other
        pcp.add_constraint(1, 2, constraint::BinaryConstraint::EQUAL);
        pcp.add_constraint(2, 3, constraint::BinaryConstraint::EQUAL);
        // Force center to be equal to first leaf (contradiction)
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(!satisfiable && "Over-constrained star should be unsatisfiable");
    },
    
    // Test 14: ALL constraint type - should always be satisfiable
    []() -> void {
        std::vector<pcp::BinaryDomain> bits = {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ANY),
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ANY)
        };
        pcp::BinaryCSP pcp(std::move(bits));
        pcp.add_constraint(0, 1, constraint::BinaryConstraint::ANY);
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "ANY constraint should always be satisfiable");
    },
    
    // Test 15: Large satisfiable case
    []() -> void {
        const int N = 100;
        std::vector<pcp::BinaryDomain> bits(N, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ANY));
        pcp::BinaryCSP pcp(std::move(bits));
        
        // Add some constraints but keep it satisfiable
        for (int i = 0; i < N - 1; i += 2) {
            pcp.add_constraint(i, i + 1, constraint::BinaryConstraint::NOTEQUAL);
        }
        
        bool satisfiable = pcpp::check_BinaryCSP_satisfiability(pcp);
        assert(satisfiable && "Large BinaryCSP with ANY domain should be satisfiable");
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        // Run each test case multiple times to catch edge cases
        for (int trial = 0; trial < 3; ++trial) {
            test_cases[i]();
        }
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
