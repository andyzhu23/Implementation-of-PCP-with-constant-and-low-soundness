#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <random>

#include "pcpp/PseudoPCPP/PseudoTester.hpp"
#include "pcpp/PseudoPCPP/CSPSolver.hpp"
#include "constraint/BitConstraint.hpp"

// Helper function to check if current assignment satisfies all constraints
// Returns true if all constraints are satisfied, false otherwise
bool checkCurrentAssignmentSatisfiability(const pcp::BitPCP &bitpcp) {
    for (const auto &[u, v, c] : bitpcp.get_constraints_list()) {
        if (!constraint::evaluateBitConstraint(c, bitpcp.get_variable(u), bitpcp.get_variable(v))) {
            return false;
        }
    }
    return true;
}

// Generate a random BitPCP with given number of variables and constraints
pcp::BitPCP generateRandomBitPCP(size_t num_variables, size_t num_constraints, std::mt19937& rng) {
    // Create BitPCP with random variable values
    std::vector<pcp::BitDomain> variables;
    std::uniform_int_distribution<int> value_dist(0, 7); // 3-bit values: 0-7
    
    for (size_t i = 0; i < num_variables; ++i) {
        variables.push_back(pcp::BitDomain(value_dist(rng)));
    }
    
    pcp::BitPCP bitpcp(std::move(variables));
    
    // Add random constraints
    std::uniform_int_distribution<size_t> var_dist(0, num_variables - 1);
    std::uniform_int_distribution<int> constraint_dist(1, 5); // Exclude ANY constraint for meaningful testing
    
    for (size_t i = 0; i < num_constraints; ++i) {
        pcp::Variable var1, var2;
        do {
            var1 = var_dist(rng);
            var2 = var_dist(rng);
        } while (var1 == var2); // Ensure different variables
        
        constraint::BitConstraint constraint_type;
        switch (constraint_dist(rng)) {
            case 1: constraint_type = constraint::BitConstraint::EQUAL; break;
            case 2: constraint_type = constraint::BitConstraint::NOTEQUAL; break;
            case 3: constraint_type = constraint::BitConstraint::FIRST_BIT_EQUAL; break;
            case 4: constraint_type = constraint::BitConstraint::SECOND_BIT_EQUAL; break;
            case 5: constraint_type = constraint::BitConstraint::THIRD_BIT_EQUAL; break;
        }
        
        bitpcp.add_constraint(var1, var2, constraint_type);
    }
    
    return bitpcp;
}

std::vector<std::function<void()>> test_cases = {
    // Test 1: Simple satisfiable BitPCP
    []() -> void {
        pcp::BitPCP original_bitpcp({0, 0, 0});
        original_bitpcp.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        original_bitpcp.add_constraint(1, 2, constraint::BitConstraint::EQUAL);
        original_bitpcp.add_constraint(0, 2, constraint::BitConstraint::EQUAL);

        pcp::BitPCP original_copy = original_bitpcp;
        bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
        
        pcpp::PseudoTester tester;
        tester.create_tester(original_bitpcp);
        pcp::BitPCP result_bitpcp = tester.buildBitPCP();
        bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
        
        std::cout << "Test 1 - Original satisfiable: " << original_satisfiable 
                  << ", Result satisfiable: " << result_satisfiable << std::endl;
        
        assert(original_satisfiable == result_satisfiable && 
               "Satisfiability should be preserved through PseudoTester");
    },
    
    // Test 2: Simple unsatisfiable BitPCP
    []() -> void {
        pcp::BitPCP original_bitpcp({0, 1});
        original_bitpcp.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        original_bitpcp.add_constraint(0, 1, constraint::BitConstraint::NOTEQUAL);

        pcp::BitPCP original_copy = original_bitpcp;
        bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
        
        pcpp::PseudoTester tester;
        tester.create_tester(original_bitpcp);
        pcp::BitPCP result_bitpcp = tester.buildBitPCP();
        bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
        
        std::cout << "Test 2 - Original satisfiable: " << original_satisfiable 
                  << ", Result satisfiable: " << result_satisfiable << std::endl;
        
        assert(original_satisfiable == result_satisfiable && 
               "Satisfiability should be preserved through PseudoTester");
    },
    
    // Test 3: Random BitPCPs with 3 variables
    []() -> void {
        std::mt19937 rng(12345);
        int test_count = 50;
        int passed = 0;
        
        for (int i = 0; i < test_count; ++i) {
            pcp::BitPCP original_bitpcp = generateRandomBitPCP(3, 2, rng);
            pcp::BitPCP original_copy = original_bitpcp;
            bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
            
            pcpp::PseudoTester tester;
            tester.create_tester(original_bitpcp);
            pcp::BitPCP result_bitpcp = tester.buildBitPCP();
            bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
            
            if (original_satisfiable == result_satisfiable) {
                passed++;
            }
        }
        
        std::cout << "Test 3 - Random 3-variable BitPCPs: " << passed << "/" << test_count 
                  << " tests passed" << std::endl;
        
        assert(passed == test_count && 
               "All random BitPCPs should preserve satisfiability through PseudoTester");
    },
    
    // Test 4: Random BitPCPs with 4 variables
    []() -> void {
        std::mt19937 rng(54321);
        int test_count = 30;
        int passed = 0;
        
        for (int i = 0; i < test_count; ++i) {
            pcp::BitPCP original_bitpcp = generateRandomBitPCP(4, 3, rng);
            pcp::BitPCP original_copy = original_bitpcp;
            bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
            
            pcpp::PseudoTester tester;
            tester.create_tester(original_bitpcp);
            pcp::BitPCP result_bitpcp = tester.buildBitPCP();
            bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
            
            if (original_satisfiable == result_satisfiable) {
                passed++;
            }
        }
        
        std::cout << "Test 4 - Random 4-variable BitPCPs: " << passed << "/" << test_count 
                  << " tests passed" << std::endl;
        
        assert(passed == test_count && 
               "All random BitPCPs should preserve satisfiability through PseudoTester");
    },
    
    // Test 5: Random BitPCPs with varying sizes
    []() -> void {
        std::mt19937 rng(67890);
        int test_count = 100;
        int passed = 0;
        
        std::uniform_int_distribution<size_t> size_dist(2, 6);
        
        for (int i = 0; i < test_count; ++i) {
            size_t num_vars = size_dist(rng);
            size_t num_constraints = std::min(num_vars, (size_t)4);
            
            pcp::BitPCP original_bitpcp = generateRandomBitPCP(num_vars, num_constraints, rng);
            pcp::BitPCP original_copy = original_bitpcp;
            bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
            
            pcpp::PseudoTester tester;
            tester.create_tester(original_bitpcp);
            pcp::BitPCP result_bitpcp = tester.buildBitPCP();
            bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
            
            if (original_satisfiable == result_satisfiable) {
                passed++;
            }
        }
        
        std::cout << "Test 5 - Random varying-size BitPCPs: " << passed << "/" << test_count 
                  << " tests passed" << std::endl;
        
        assert(passed == test_count && 
               "All random BitPCPs should preserve satisfiability through PseudoTester");
    },
    
    // Test 6: Edge case - Empty BitPCP (no constraints)
    []() -> void {
        pcp::BitPCP original_bitpcp(3);  // 3 variables, no constraints
        pcp::BitPCP original_copy = original_bitpcp;
        bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
        
        pcpp::PseudoTester tester;
        tester.create_tester(original_bitpcp);
        pcp::BitPCP result_bitpcp = tester.buildBitPCP();
        bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
        
        std::cout << "Test 6 - Empty BitPCP: Original satisfiable: " << original_satisfiable 
                  << ", Result satisfiable: " << result_satisfiable << std::endl;
        
        assert(original_satisfiable == result_satisfiable && 
               "Empty BitPCP should preserve satisfiability through PseudoTester");
    },
    
    // Test 7: Complex satisfiable BitPCP with bit-specific constraints
    []() -> void {
        pcp::BitPCP original_bitpcp({1, 3, 5}); // Binary: 001, 011, 101
        original_bitpcp.add_constraint(0, 1, constraint::BitConstraint::FIRST_BIT_EQUAL); // bit 0: 0 == 0 ✓
        original_bitpcp.add_constraint(1, 2, constraint::BitConstraint::SECOND_BIT_EQUAL); // bit 1: 1 == 0 ✗
        
        // Let's make it satisfiable by adjusting variables
        original_bitpcp.set_variable(2, pcp::BitDomain(7)); // 111, so bit 1: 1 == 1 ✓
        
        pcp::BitPCP original_copy = original_bitpcp;
        bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
        
        pcpp::PseudoTester tester;
        tester.create_tester(original_bitpcp);
        pcp::BitPCP result_bitpcp = tester.buildBitPCP();
        bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
        
        std::cout << "Test 7 - Complex bit-specific constraints: Original satisfiable: " 
                  << original_satisfiable << ", Result satisfiable: " << result_satisfiable << std::endl;
        
        assert(original_satisfiable == result_satisfiable && 
               "Complex BitPCP should preserve satisfiability through PseudoTester");
    },
    
    // Test 8: Stress test with larger random BitPCPs
    []() -> void {
        std::mt19937 rng(13579);
        int test_count = 20;
        int passed = 0;
        
        for (int i = 0; i < test_count; ++i) {
            pcp::BitPCP original_bitpcp = generateRandomBitPCP(8, 6, rng);
            pcp::BitPCP original_copy = original_bitpcp;
            bool original_satisfiable = pcpp::check_bitPCP_satisfiability(original_copy);
            
            pcpp::PseudoTester tester;
            tester.create_tester(original_bitpcp);
            pcp::BitPCP result_bitpcp = tester.buildBitPCP();
            bool result_satisfiable = checkCurrentAssignmentSatisfiability(result_bitpcp);
            
            if (original_satisfiable == result_satisfiable) {
                passed++;
            }
        }
        
        std::cout << "Test 8 - Large random BitPCPs (8 vars): " << passed << "/" << test_count 
                  << " tests passed" << std::endl;
        
        assert(passed == test_count && 
               "Large random BitPCPs should preserve satisfiability through PseudoTester");
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