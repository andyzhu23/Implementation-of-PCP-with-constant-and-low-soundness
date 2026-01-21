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
        if (constraint != Constraint::ANY) {
            pcp::BitDomain value(assignment[var1], assignment[var2], assignment[var3], constraint);
            bitpcp.add_variable(value);

            bitpcp.add_constraint(var1, bitpcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
            bitpcp.add_constraint(var2, bitpcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
            bitpcp.add_constraint(var3, bitpcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
        }
    }

    return bitpcp;
}

pcp::BitPCP ThreeCSP::toBitPCP(
    pcp::BitPCP original_pcp,
    const std::unordered_map<size_t, std::pair<size_t, std::optional<size_t>>> &three_csp_to_pcp_index,
    size_t pcp_in_three_csp_size
) const {

    size_t original_size = original_pcp.get_size();
    
    pcp::BitPCP pcp = std::move(original_pcp);
    
    // Add variables
    for (size_t i = pcp_in_three_csp_size; i < assignment.size(); ++i) {
        pcp::BitDomain bitdomain(assignment[i], assignment[i], assignment[i], Constraint::ENCODED_BINARY);
        pcp.add_variable(bitdomain);
    }

    auto get_new_index = [&](size_t three_csp_index) -> std::pair<size_t, std::optional<size_t>> {
        if (three_csp_index < pcp_in_three_csp_size) {
            auto it = three_csp_to_pcp_index.find(three_csp_index);
            if (it != three_csp_to_pcp_index.end()) {
                return it->second; // map to the first bit index in PCP
            } else {
                throw std::invalid_argument("ThreeCSP::toBitPCP: three_csp_index not found in mapping");
            }
        } else {
            return {three_csp_index - pcp_in_three_csp_size + original_size, std::nullopt}; // new variable index in PCP
        }
    };

    // Add binary constraints
    for (auto [var1, var2, bit_constraint] : binary_constraints) {
        auto [new_var1, var1_offset] = get_new_index(var1);
        auto [new_var2, var2_offset] = get_new_index(var2);
        pcp.add_constraint(new_var1, new_var2, bit_constraint);
    }

    // Add ternary constraints
    for (const auto &[var1, var2, var3, constraint] : ternary_constraints) {
        // ENCODED_BINARY is enforced by binary constraint assignment itself, ANY means no constraint
        if (constraint != Constraint::ANY) {

            auto [new_var1, var1_offset] = get_new_index(var1);
            auto [new_var2, var2_offset] = get_new_index(var2);
            auto [new_var3, var3_offset] = get_new_index(var3);

            if (pcp.get_variable(new_var2).get_domain_type() != Constraint::ENCODED_BINARY ||
                pcp.get_variable(new_var3).get_domain_type() != Constraint::ENCODED_BINARY) {
                throw std::invalid_argument("ThreeCSP::toBitPCP: Ternary constraint on non ENCODED_BINARY variable");
            }
            
            if (var1_offset == std::nullopt) {
                // add var1 as first bit in new variable
                pcp::BitDomain value(assignment[var1], assignment[var2], assignment[var3], constraint);
                pcp.add_variable(value);
                pcp.add_constraint(new_var1, pcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
                pcp.add_constraint(new_var2, pcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
                pcp.add_constraint(new_var3, pcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
            } else if (*var1_offset == 0) {
                // add var1 as first bit in new variable
                pcp::BitDomain value(assignment[var1], assignment[var2], assignment[var3], constraint);
                pcp.add_variable(value);
                pcp.add_constraint(new_var1, pcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
                pcp.add_constraint(new_var2, pcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
                pcp.add_constraint(new_var3, pcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
            } else if (*var1_offset == 1) {
                // add var1 as second bit in new variable
                pcp::BitDomain value(assignment[var2], assignment[var1], assignment[var3], constraint);
                pcp.add_variable(value);
                pcp.add_constraint(new_var1, pcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
                pcp.add_constraint(new_var2, pcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
                pcp.add_constraint(new_var3, pcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
            } else if (*var1_offset == 2) {
                // add var1 as third bit in new variable
                pcp::BitDomain value(assignment[var2], assignment[var3], assignment[var1], constraint);
                pcp.add_variable(value);
                pcp.add_constraint(new_var1, pcp.get_size() - 1, constraint::BitConstraint::THIRD_BIT_EQUAL);
                pcp.add_constraint(new_var2, pcp.get_size() - 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
                pcp.add_constraint(new_var3, pcp.get_size() - 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
            }
            
        }
    }

    return pcp;
}

}