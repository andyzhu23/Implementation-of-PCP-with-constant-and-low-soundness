/*
 * Author: Andy Zhu
 * @date    2025-11-25 16:47:34
 * @version 1.0.0
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <numeric>

#include "hadamard/Tester.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "pcp/Aliases.hpp"

// Helper function to check if a given assignment satisfies a BitPCP
bool check_assignment(const pcp::BitPCP& pcp, const std::vector<pcp::BitDomain>& assignment) {
    if (pcp.get_size() != assignment.size()) {
        return false;
    }

    for (const auto& constr : pcp.get_constraints_list()) {
        auto [u, v, c] = constr;
        if (c == constraint::BitConstraint::ANY) continue;
        
        bool val_u = assignment[u];
        bool val_v = assignment[v];
        bool satisfied;
        if (c == constraint::BitConstraint::EQUAL) {
            satisfied = (val_u == val_v);
        } else if (c == constraint::BitConstraint::NOTEQUAL) {
            satisfied = (val_u != val_v);
        }
        if (!satisfied) {
            return false;
        }
    }
    return true;
}

// Helper to build the intended assignment for the generated BitPCP
std::vector<pcp::BitDomain> build_intended_assignment(
    const pcp::PoweringDomain& u, 
    const pcp::PoweringDomain& v) {
    
    std::vector<pcp::BitDomain> assignment;
    assignment.insert(assignment.end(), u.begin(), u.end());
    assignment.insert(assignment.end(), v.begin(), v.end());
    assignment.push_back(1);
    
    pcpp::Hadamard h(assignment);
    assignment.insert(assignment.end(), h.getCode().begin(), h.getCode().end());
    assignment.push_back(0);
    return assignment;
}

std::vector<std::function<void()>> test_cases = {
    // Test 1: Basic test
    []() -> void {
        pcp::PoweringDomain u = {1, 0};
        pcp::PoweringDomain v = {0, 1};
        constraint::PoweringConstraint constraint(u.size());
        constraint.add_constraint(0, 1, constraint::BitConstraint::EQUAL);

        pcpp::Tester tester(u, v, constraint);
        int sampling_coeff = 10;
        pcp::BitPCP pcp = tester.buildBitPCP(sampling_coeff);

        size_t assignment_size = u.size() + v.size() + 1;
        size_t hadamard_size = 1 << assignment_size;
        size_t expected_pcp_size = assignment_size + hadamard_size + 1;

        std::cout << "PCP size: " << pcp.get_size() << ", Expected size: " << expected_pcp_size << std::endl;

        assert(pcp.get_size() == expected_pcp_size);

        // Check the number of constraints.
        // It should be assignment_size from the Hadamard code mapping
        // plus sampling_coeff from the linearity tests.
        // assert(pcp.get_constraints_list().size() == assignment_size + sampling_coeff);
    },
    // Test 2: No constraints
    []() -> void {
        pcp::PoweringDomain u = {1};
        pcp::PoweringDomain v = {0};
        constraint::PoweringConstraint constraint(u.size());

        pcpp::Tester tester(u, v, constraint);
        int sampling_coeff = 5;
        pcp::BitPCP pcp = tester.buildBitPCP(sampling_coeff);

        size_t assignment_size = u.size() + v.size() + 1;
        size_t hadamard_size = 1 << assignment_size;
        size_t expected_pcp_size = assignment_size + hadamard_size + 1;

        assert(pcp.get_size() == expected_pcp_size);
        // assert(pcp.get_constraints_list().size() == assignment_size + sampling_coeff);
    },
    // Test 3: Satisfiable input constraints
    []() -> void {
        pcp::PoweringDomain u = {1, 0, 1};
        pcp::PoweringDomain v = {0, 1, 1};
        constraint::PoweringConstraint constraint(u.size());
        // These constraints are satisfied by u
        constraint.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        constraint.add_constraint(0, 2, constraint::BitConstraint::EQUAL);
        constraint.add_constraint(1, 0, constraint::BitConstraint::EQUAL);
        constraint.add_constraint(1, 2, constraint::BitConstraint::NOTEQUAL);
        constraint.add_constraint(2, 0, constraint::BitConstraint::NOTEQUAL);
        constraint.add_constraint(2, 2, constraint::BitConstraint::EQUAL);

        pcpp::Tester tester(u, v, constraint);
        pcp::BitPCP pcp = tester.buildBitPCP(20);

        std::vector<pcp::BitDomain> pcp_assignment = build_intended_assignment(u, v);
        assert(check_assignment(pcp, pcp_assignment) && "PCP from satisfiable assignment should be satisfied by that assignment");
    },
    // Test 4: Unsatisfiable input constraints
    []() -> void {
        pcp::PoweringDomain u = {1, 0, 1};
        pcp::PoweringDomain v = {0, 1, 1};
        constraint::PoweringConstraint constraint(u.size());
        // This constraint is NOT satisfied by u
        constraint.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        constraint.add_constraint(1, 1, constraint::BitConstraint::NOTEQUAL);
        constraint.add_constraint(2, 1, constraint::BitConstraint::NOTEQUAL); // violated constraint
        constraint.add_constraint(0, 0, constraint::BitConstraint::NOTEQUAL);
        constraint.add_constraint(2, 2, constraint::BitConstraint::EQUAL);

        pcpp::Tester tester(u, v, constraint);
        pcp::BitPCP pcp = tester.buildBitPCP(20);

        std::vector<pcp::BitDomain> pcp_assignment = build_intended_assignment(u, v);
        assert(!check_assignment(pcp, pcp_assignment) && "PCP from unsatisfiable assignment should not be satisfied by that assignment");
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
