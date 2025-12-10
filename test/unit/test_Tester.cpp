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

        // New Tester implementation samples linear combinations of the constraint rows
        // and produces one variable per unique sampled position (plus the initial 0 variable).
        // We can reconstruct the set of positions deterministically here and assert the
        // produced BitPCP has the matching number of variables and constraints.
        size_t assignment_size = u.size() + v.size() + 1;

        // Build constraint rows the same way as Tester does: a row is a bitmask
        // over the assignment positions with ones at the two variable indices and
        // the last bit set based on EQUAL/NOTEQUAL. We'll represent rows as
        // integer bitmasks and compute all XOR combinations to find unique positions.
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

        // enumerate all subsets of rows and xor their masks to get positions
        std::unordered_set<uint64_t> positions;
        size_t m = rows.size();
        for (uint64_t sample = 0; sample < (1ULL << m); ++sample) {
            uint64_t pos = 0;
            for (size_t j = 0; j < m; ++j) {
                if ((sample >> j) & 1ULL) pos ^= rows[j];
            }
            positions.insert(pos);
        }

        size_t expected_pcp_size = 1 + positions.size(); // initial 0 var + unique positions
        std::cout << "PCP size: " << pcp.get_size() << ", Expected size: " << expected_pcp_size << std::endl;
        assert(pcp.get_size() == expected_pcp_size);

        // Each unique position becomes a constraint equating it to the 0 variable.
        assert(pcp.get_constraints_list().size() == positions.size());
        for (const auto &t : pcp.get_constraints_list()) {
            auto [a, b, c] = t;
            assert(a == 0);
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
        size_t expected_pcp_size = 1 + positions.size();
        assert(pcp.get_size() == expected_pcp_size);
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
        size_t expected_pcp_size = 1 + positions.size();
        assert(pcp.get_size() == expected_pcp_size);
        assert(pcp.get_constraints_list().size() == positions.size());
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
        size_t expected_pcp_size = 1 + positions.size();
        assert(pcp.get_size() == expected_pcp_size);
        assert(pcp.get_constraints_list().size() == positions.size());
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
