#include "three_csp/ThreeCSP.hpp"
#include "pcp/BitDomain.hpp"

namespace three_csp {

void ThreeCSP::add_ternary_constraint(size_t var1, size_t var2, size_t var3, Constraint constraint) {
    ternary_constraints.emplace_back(var1, var2, var3, constraint);
}

void ThreeCSP::add_binary_constraint(size_t var1, size_t var2, constraint::BitConstraint constraint) {
    binary_constraints.emplace_back(var1, var2, constraint);
}

void ThreeCSP::add_variable(Domain value) {
    assignment.push_back(value);
}

const std::vector<Domain>& ThreeCSP::get_assignment() const {
    return assignment;
}

Domain ThreeCSP::get_variable(size_t index) const {
    return assignment[index];
}

size_t ThreeCSP::size() const {
    return assignment.size();
}

pcp::BitPCP ThreeCSP::toBitPCP() const {
    pcp::BitPCP bitpcp;
    // Add variables
    for (Domain val : assignment) {
        pcp::BitDomain bitdomain(val, val, val, Constraint::ENCODED_BINARY);
        bitpcp.add_variable(bitdomain);
    }
    // Add binary constraints
    for (const auto &[var1, var2, bit_constraint] : binary_constraints) {
        bitpcp.add_constraint(var1, var2, bit_constraint);
    }

    // Add ternary constraints
    for (const auto &[var1, var2, var3, constraint] : ternary_constraints) {
        // ENCODED_BINARY is enforced by binary constraint assignment itself, ANY means no constraint
        if (constraint != Constraint::ENCODED_BINARY && constraint != Constraint::ANY) {
            pcp::BitDomain value(assignment[var1], assignment[var2], assignment[var3], constraint);
            bitpcp.add_variable(value);

            bitpcp.add_constraint(var1, bitpcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
            bitpcp.add_constraint(var2, bitpcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
            bitpcp.add_constraint(var3, bitpcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
        }
    }

    return bitpcp;
}

}