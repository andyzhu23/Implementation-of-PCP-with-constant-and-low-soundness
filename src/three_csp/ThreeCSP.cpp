#include "three_csp/ThreeCSP.hpp"
#include "pcp/BitPCP.hpp"
#include "constraint/BitConstraint.hpp"

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

pcp::BitPCP ThreeCSP::toBitPCP() {
    pcp::BitPCP bitpcp;
    while (assignment.size() % 3 != 0) {
        // pad to make size multiple of 3
        assignment.push_back(assignment.back());
    }
    for (size_t i = 0; i < assignment.size(); i+=3) {
        pcp::BitDomain bitdomain(assignment[i], assignment[i + 1], assignment[i + 2], Constraint::ANY);
        bitpcp.add_variable(bitdomain);
    }

    // Add ternary constraints
    for (const auto &[var1, var2, var3, constraint] : ternary_constraints) {
        if (var1 % 3 == 0 && var2 % 3 == 1 && var3 % 3 == 2 && var1 / 3 == var2 / 3 && var2 / 3 == var3 / 3) {
            // already in the correct format
            bitpcp.set_variable(var1 / 3, pcp::BitDomain(
                assignment[var1],
                assignment[var2],
                assignment[var3],
                constraint
            ));
        } else {
            // need to create a new variable to represent the ternary constraint
            pcp::BitDomain value(assignment[var1], assignment[var2], assignment[var3], constraint);

            bitpcp.add_variable(value);

            switch (var1 % 3) {
                case 0:
                    bitpcp.add_constraint(var1 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
                    break;
                case 1:
                    bitpcp.add_constraint(var1 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL_FIRST_BIT);
                    break;
                case 2:
                    bitpcp.add_constraint(var1 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL_FIRST_BIT);
                    break;
            }

            switch (var2 % 3) {
                case 0:
                    bitpcp.add_constraint(var2 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL_SECOND_BIT);
                    break;
                case 1:
                    bitpcp.add_constraint(var2 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
                    break;
                case 2:
                    bitpcp.add_constraint(var2 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL_SECOND_BIT);
                    break;
            }

            switch (var3 % 3) {
                case 0:
                    bitpcp.add_constraint(var3 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL_THIRD_BIT);
                    break;
                case 1:
                    bitpcp.add_constraint(var3 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL_THIRD_BIT);
                    break;
                case 2:
                    bitpcp.add_constraint(var3 / 3, bitpcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
                    break;
            }
        }
    }

    // Add binary constraints
    for (const auto &[var1, var2, bit_constraint] : binary_constraints) {
        switch (bit_constraint) {
            case constraint::BitConstraint::EQUAL:
                switch (var1 % 3) {
                    case 0:
                        switch (var2 % 3) {
                            case 0:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::FIRST_BIT_EQUAL);
                                break;
                            case 1:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::FIRST_BIT_EQUAL_SECOND_BIT);
                                break;
                            case 2:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::FIRST_BIT_EQUAL_THIRD_BIT);
                                break;
                        }
                        break;
                    case 1:
                        switch (var2 % 3) {
                            case 0:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::SECOND_BIT_EQUAL_FIRST_BIT);
                                break;
                            case 1:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::SECOND_BIT_EQUAL);
                                break;
                            case 2:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::SECOND_BIT_EQUAL_THIRD_BIT);
                                break;
                        }
                        break;
                    case 2:
                        switch (var2 % 3) {
                            case 0:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::THIRD_BIT_EQUAL_FIRST_BIT);
                                break;
                            case 1:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::THIRD_BIT_EQUAL_SECOND_BIT);
                                break;
                            case 2:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::THIRD_BIT_EQUAL);
                                break;
                        }
                        break;
                }
                break;
            case constraint::BitConstraint::NOTEQUAL:
                switch (var1 % 3) {
                    case 0:
                        switch (var2 % 3) {
                            case 0:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::FIRST_BIT_NOTEQUAL);
                                break;
                            case 1:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::FIRST_BIT_NOTEQUAL_SECOND_BIT);
                                break;
                            case 2:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::FIRST_BIT_NOTEQUAL_THIRD_BIT);
                                break;
                        }
                        break;
                    case 1:
                        switch (var2 % 3) {
                            case 0:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::SECOND_BIT_NOTEQUAL_FIRST_BIT);
                                break;
                            case 1:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::SECOND_BIT_NOTEQUAL);
                                break;
                            case 2:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::SECOND_BIT_NOTEQUAL_THIRD_BIT);
                                break;
                        }
                        break;
                    case 2:
                        switch (var2 % 3) {
                            case 0:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::THIRD_BIT_NOTEQUAL_FIRST_BIT);
                                break;
                            case 1:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::THIRD_BIT_NOTEQUAL_SECOND_BIT);
                                break;
                            case 2:
                                bitpcp.add_constraint(var1 / 3, var2 / 3, constraint::BitConstraint::THIRD_BIT_NOTEQUAL);
                                break;
                        }
                        break;
                }
                break;
            default:
                // other constraints are not possible in this context
                break;
        }
    }

    return bitpcp;
}

}