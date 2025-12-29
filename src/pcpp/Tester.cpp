#include <chrono>
#include <random>
#include <unordered_map>

#include "constants.hpp"
#include "pcpp/Tester.hpp"
#include "pcp/Aliases.hpp"
#include "three_color/three_color.hpp"

namespace pcpp {

Tester::Tester(pcp::PoweringDomain u, pcp::PoweringDomain v, constraint::PoweringConstraint constraint) {
    // Reserve variables to hold u, v, and one extra for negation
    assignment.reserve(u.size() + v.size() + 1);

    // Initialize variables with u values
    for (pcp::BitDomain bit : u) {
        assignment.push_back(bit);
    }

    // Initialize variables with v values
    for (pcp::BitDomain bit : v) {
        assignment.push_back(bit);
    }

    assignment.push_back(1); // The last variable is always 1 for negation purposes.

    // Initialize Hadamard code with the variable assignments
    hadamard = Hadamard(assignment);

    for (size_t i = 0; i < u.size(); ++i) {
        for (const auto[j, bit_constraint] : constraint.get_constraints(i)) {
            if (bit_constraint != constraint::BitConstraint::ANY) {
                std::vector<pcp::BitDomain> row(assignment.size());
                // set the variable positions to 1 indicates using them in the constraint
                row[i] = row[j + u.size()] = 1;
                // set the last position to 0 or 1 based on EQUAL or NOTEQUAL
                row.back() = (bit_constraint == constraint::BitConstraint::EQUAL) ? 0 : 1;
                constraint_matrix.push_back(row);
            }
        }
    }
}

const int THREE_COLOR_ASSIGNMENT = 2 + 2 + 2 * 2 + 1; // 2 bits per color, 4 products, and 1 for negation

Tester::Tester(three_color::Color u, three_color::Color v) : assignment(THREE_COLOR_ASSIGNMENT) {
    std::bitset<2> ubits = three_color::color_to_bits(u);
    std::bitset<2> vbits = three_color::color_to_bits(v);
    std::bitset<2> bit0;
    bit0[0] = ubits[0];
    bit0[1] = vbits[0];
    std::bitset<2> bit1;
    bit1[0] = ubits[1];
    bit1[1] = vbits[1];

    // stores 3 bits per variable, and their products
    // hardcode products
    assignment[0] = bit0[0];
    assignment[1] = bit0[1];
    assignment[2] = bit1[0];
    assignment[3] = bit1[1];
    assignment[4] = bit0[0] * bit1[0];
    assignment[5] = bit0[0] * bit1[1];
    assignment[6] = bit0[1] * bit1[0];
    assignment[7] = bit0[1] * bit1[1];
    assignment[8] = 1;

    hadamard = Hadamard(assignment);

    constraint_matrix = std::vector<std::vector<pcp::BitDomain>>{
        // the local assignment u and v have different colors
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
}

// Construct Tester from a BitPCP
Tester::Tester(pcp::BitPCP pcp) : assignment(pcp.get_size() + 1) {
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        assignment[i] = pcp.get_variable(i);
    }

    assignment.back() = constants::PCPVARIABLE_ONE; // extra variable for negation

    hadamard = Hadamard(assignment);
    // Build constraint matrix from BitPCP constraints
    for (const auto &[var1, var2, bit_constraint] : pcp.get_constraints_list()) {
        if (bit_constraint != constraint::BitConstraint::ANY) {
            std::vector<pcp::BitDomain> row(assignment.size());
            row[var1] = constants::PCPVARIABLE_ONE;
            row[var2] = constants::PCPVARIABLE_ONE;
            row.back() = (bit_constraint == constraint::BitConstraint::EQUAL) ? 0 : 1;
            constraint_matrix.push_back(std::move(row));
        }
    }
}

pcp::BitPCP Tester::buildBitPCP() {
    std::vector<pcp::BitDomain> variables(1);

    std::unordered_map<pcp::Variable, size_t> used_positions;

    std::uniform_int_distribution<size_t> dist(0, (constants::PCPVARIABLE_ONE << constraint_matrix.size()) - 1);

    for (size_t _ = 0; _ < constants::LINEARITY_COEFFICIENT; ++_) {
        // random sample
        size_t sample = dist(constants::RANDOM_SEED);
        pcp::Variable position = 0;
        for (size_t j = 0; j < constraint_matrix.size(); ++j) {
            if ((sample >> j) & 1) {
                // include this constraint in the linear combination
                for (size_t k = 0; k < constraint_matrix[j].size(); ++k) {
                    if (constraint_matrix[j][k]) {
                        position ^= (constants::PCPVARIABLE_ONE << k);
                    }
                }
            }
        }
        if (used_positions.find(position) == used_positions.end()) {
            used_positions[position] = variables.size();
            variables.push_back(hadamard.query(position));
        }
    }

    pcp::BitPCP result = std::move(variables);

    for (const auto &[pos, idx] : used_positions) {
        result.add_constraint(0, idx, constraint::BitConstraint::EQUAL);
    }

    return result;
}

}