#include <cassert>

#include "three_csp/ThreeCSP.hpp"
#include "pcp/BinaryDomain.hpp"

namespace three_csp {

void ThreeCSP::add_ternary_constraint(size_t var1, size_t var2, size_t var3, Constraint constraint) {
    ternary_constraints.emplace_back(var1, var2, var3, constraint);
}

void ThreeCSP::add_binary_constraint(size_t var1, size_t var2, constraint::BinaryConstraint constraint) {
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

pcp::BinaryCSP ThreeCSP::toBinaryCSP() const {
    pcp::BinaryCSP BinaryCSP;
    // Add variables
    for (Domain val : assignment) {
        pcp::BinaryDomain BinaryDomain(val, val, val, Constraint::ENCODED_BINARY);
        BinaryCSP.add_variable(BinaryDomain);
    }
    // Add binary constraints
    for (const auto &[var1, var2, bit_constraint] : binary_constraints) {
        BinaryCSP.add_constraint(var1, var2, bit_constraint);
    }

    // Add ternary constraints
    for (const auto &[var1, var2, var3, constraint] : ternary_constraints) {
        // ENCODED_BINARY is enforced by binary constraint assignment itself, ANY means no constraint
        if (constraint != Constraint::ENCODED_BINARY && constraint != Constraint::ANY) {
            pcp::BinaryDomain value(assignment[var1], assignment[var2], assignment[var3], constraint);
            BinaryCSP.add_variable(value);

            BinaryCSP.add_constraint(var1, BinaryCSP.get_size() - 1, constraint::BinaryConstraint::FIRST_BIT_EQUAL);
            BinaryCSP.add_constraint(var2, BinaryCSP.get_size() - 1, constraint::BinaryConstraint::SECOND_BIT_EQUAL);
            BinaryCSP.add_constraint(var3, BinaryCSP.get_size() - 1, constraint::BinaryConstraint::THIRD_BIT_EQUAL);
        }
    }

    return BinaryCSP;
}

}