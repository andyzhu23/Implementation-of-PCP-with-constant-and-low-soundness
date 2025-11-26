#include <functional>
#include <iostream>
#include <cassert>
#include <vector>

#include "core/core.hpp"
#include "pcp/PoweringPCP.hpp"
#include "pcp/BitPCP.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "constraint/BitConstraint.hpp"
#include "pcp/Aliases.hpp"
#include "pcpp/Hadamard.hpp"

// Helper: construct the intended assignment used by Tester (u, v, 1, hadamard(code), 0)
static std::vector<pcp::BitDomain> build_intended_assignment(const pcp::PoweringDomain &u, const pcp::PoweringDomain &v) {
    std::vector<pcp::BitDomain> assignment;
    assignment.insert(assignment.end(), u.begin(), u.end());
    assignment.insert(assignment.end(), v.begin(), v.end());
    assignment.push_back(1);
    pcpp::Hadamard h(assignment);
    assignment.insert(assignment.end(), h.getCode().begin(), h.getCode().end());
    assignment.push_back(0);
    return assignment;
}

// Helper: check whether a given assignment satisfies all constraints of a BitPCP
static bool check_assignment(const pcp::BitPCP &pcp, const std::vector<pcp::BitDomain> &assignment) {
    if (pcp.get_size() != assignment.size()) return false;
    for (const auto &t : pcp.get_constraints_list()) {
        auto [u, v, c] = t;
        if (c == constraint::BitConstraint::ANY) continue;
        bool val_u = assignment[u];
        bool val_v = assignment[v];
        bool sat = (c == constraint::BitConstraint::EQUAL) ? (val_u == val_v) : (val_u != val_v);
        if (!sat) return false;
    }
    return true;
}

// Helper: expected size of Tester-produced BitPCP for given u and v
static size_t expected_tester_pcp_size(const pcp::PoweringDomain &u, const pcp::PoweringDomain &v) {
    size_t assignment_size = u.size() + v.size() + 1;
    size_t hadamard_size = 1u << assignment_size;
    return assignment_size + hadamard_size + 1;
}

std::vector<std::function<void()>> test_cases = {
    // Test 1: No constraints -> result should be empty BitPCP
    []() -> void {
        pcp::PoweringDomain u = {1};
        pcp::PoweringDomain v = {0};
        pcp::PoweringPCP powering_pcp(2);
        powering_pcp.set_variables(0, u);
        powering_pcp.set_variables(1, v);

        pcp::BitPCP result = core::reduce_alphabet(powering_pcp, 5);
        assert(result.get_size() == 0 && "Expected empty BitPCP when PoweringPCP has no constraints");
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All reduce_alphabet tests passed!" << std::endl;
    return 0;
}
