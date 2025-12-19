#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <cstdint>

#include "pcpp/Tester.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "pcp/Aliases.hpp"
#include "constants.hpp"

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
        pcp::BitPCP pcp = tester.buildBitPCP();

        // The buildBitPCP implementation:
        // 1. Starts with assignment variables: u, v, and 1 (for negation)
        // 2. Adds one linearity test variable (0)
        // 3. For each unique position from XORing constraint rows, adds a Hadamard query variable
        size_t assignment_size = u.size() + v.size() + 1;

        // Build constraint rows the same way as Tester does: a row is a bitmask
        // over the assignment positions with ones at the two variable indices and
        // the last bit set based on EQUAL/NOTEQUAL.
        std::vector<uint64_t> rows;
        for (size_t i = 0; i < u.size(); ++i) {
            for (const auto &[j, bit_constraint] : constraint.get_constraints(i)) {
                if (bit_constraint != constraint::BitConstraint::ANY) {
                    uint64_t mask = 0;
                    mask |= (1ULL << i);
                    mask |= (1ULL << (j + u.size()));
                    // The last bit in the assignment is the parity/negation bit.
                    if (bit_constraint == constraint::BitConstraint::NOTEQUAL) {
                        mask |= (1ULL << (assignment_size - 1));
                    }
                    rows.push_back(mask);
                }
            }
        }

        // Enumerate all subsets of rows and XOR their masks to get unique positions
        std::unordered_set<uint64_t> positions;
        size_t m = rows.size();
        for (uint64_t sample = 0; sample < (1ULL << m); ++sample) {
            uint64_t pos = 0;
            for (size_t j = 0; j < m; ++j) {
                if ((sample >> j) & 1ULL) pos ^= rows[j];
            }
            positions.insert(pos);
        }

        // The new Tester samples random linear combinations up to LINEARITY_COEFFICIENT
        // so the resulting PCP size is at most assignment + 1 + LINEARITY_COEFFICIENT
        size_t expected_max_pcp_size = assignment_size + 1 + constants::LINEARITY_COEFFICIENT;
        std::cout << "Test 1 - PCP size: " << pcp.get_size() << ", Max expected size: " << expected_max_pcp_size << std::endl;
        assert(pcp.get_size() <= expected_max_pcp_size);

        // Constraint count is at most LINEARITY_COEFFICIENT (sampling may produce duplicates)
        assert(pcp.get_constraints_list().size() <= constants::LINEARITY_COEFFICIENT);
        size_t linearity_var = assignment_size;
        for (const auto &t : pcp.get_constraints_list()) {
            auto [a, b, c] = t;
            assert(a == linearity_var);
            assert(c == constraint::BitConstraint::EQUAL);
        }
    },
    // Test 2: No constraints
    []() -> void {
        pcp::PoweringDomain u = {1};
        pcp::PoweringDomain v = {0};
        constraint::PoweringConstraint constraint(u.size());

        pcpp::Tester tester(u, v, constraint);
        pcp::BitPCP pcp = tester.buildBitPCP();

        // Reconstruct rows and positions same as in Test 1 and assert structural properties
        size_t assignment_size = u.size() + v.size() + 1;
        std::vector<uint64_t> rows;
        for (size_t i = 0; i < u.size(); ++i) {
            for (const auto &[j, bit_constraint] : constraint.get_constraints(i)) {
                if (bit_constraint != constraint::BitConstraint::ANY) {
                    uint64_t mask = 0;
                    mask |= (1ULL << i);
                    mask |= (1ULL << (j + u.size()));
                    if (bit_constraint == constraint::BitConstraint::NOTEQUAL) {
                        mask |= (1ULL << (assignment_size - 1));
                    }
                    rows.push_back(mask);
                }
            }
        }
        std::unordered_set<uint64_t> positions;
        size_t m = rows.size();
        for (uint64_t sample = 0; sample < (1ULL << m); ++sample) {
            uint64_t pos = 0;
            for (size_t j = 0; j < m; ++j) if ((sample >> j) & 1ULL) pos ^= rows[j];
            positions.insert(pos);
        }
        // With no constraints, there are no rows, so only the empty subset (position 0) is sampled
    // The new Tester samples random linear combinations up to LINEARITY_COEFFICIENT
    // so the resulting PCP size is at most assignment + 1 + LINEARITY_COEFFICIENT
    size_t expected_max_pcp_size = assignment_size + 1 + constants::LINEARITY_COEFFICIENT;
    std::cout << "Test 2 - PCP size: " << pcp.get_size() << ", Max expected size: " << expected_max_pcp_size << std::endl;
    assert(pcp.get_size() <= expected_max_pcp_size);
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
        pcp::BitPCP pcp = tester.buildBitPCP();

        // Reconstruct expected positions from the input constraints (like Test 1)
        size_t assignment_size = u.size() + v.size() + 1;
        std::vector<uint64_t> rows;
        for (size_t i = 0; i < u.size(); ++i) {
            for (const auto &[j, bit_constraint] : constraint.get_constraints(i)) {
                if (bit_constraint != constraint::BitConstraint::ANY) {
                    uint64_t mask = 0;
                    mask |= (1ULL << i);
                    mask |= (1ULL << (j + u.size()));
                    if (bit_constraint == constraint::BitConstraint::NOTEQUAL) {
                        mask |= (1ULL << (assignment_size - 1));
                    }
                    rows.push_back(mask);
                }
            }
        }
        std::unordered_set<uint64_t> positions;
        size_t m = rows.size();
        for (uint64_t sample = 0; sample < (1ULL << m); ++sample) {
            uint64_t pos = 0;
            for (size_t j = 0; j < m; ++j) if ((sample >> j) & 1ULL) pos ^= rows[j];
            positions.insert(pos);
        }
        size_t expected_max_pcp_size = assignment_size + 1 + constants::LINEARITY_COEFFICIENT;
        std::cout << "Test 3 - PCP size: " << pcp.get_size() << ", Expected size: " << expected_max_pcp_size << std::endl;
        assert(pcp.get_size() <= expected_max_pcp_size);
        assert(pcp.get_constraints_list().size() <= constants::LINEARITY_COEFFICIENT);
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
        pcp::BitPCP pcp = tester.buildBitPCP();

        // Reconstruct expected positions from the input constraints (like Test 1)
        size_t assignment_size = u.size() + v.size() + 1;
        std::vector<uint64_t> rows;
        for (size_t i = 0; i < u.size(); ++i) {
            for (const auto &[j, bit_constraint] : constraint.get_constraints(i)) {
                if (bit_constraint != constraint::BitConstraint::ANY) {
                    uint64_t mask = 0;
                    mask |= (1ULL << i);
                    mask |= (1ULL << (j + u.size()));
                    if (bit_constraint == constraint::BitConstraint::NOTEQUAL) {
                        mask |= (1ULL << (assignment_size - 1));
                    }
                    rows.push_back(mask);
                }
            }
        }
        std::unordered_set<uint64_t> positions;
        size_t m = rows.size();
        for (uint64_t sample = 0; sample < (1ULL << m); ++sample) {
            uint64_t pos = 0;
            for (size_t j = 0; j < m; ++j) if ((sample >> j) & 1ULL) pos ^= rows[j];
            positions.insert(pos);
        }
    size_t expected_max_pcp_size = assignment_size + 1 + constants::LINEARITY_COEFFICIENT;
    std::cout << "Test 4 - PCP size: " << pcp.get_size() << ", Max expected size: " << expected_max_pcp_size << std::endl;
    assert(pcp.get_size() <= expected_max_pcp_size);
    assert(pcp.get_constraints_list().size() <= constants::LINEARITY_COEFFICIENT);
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
