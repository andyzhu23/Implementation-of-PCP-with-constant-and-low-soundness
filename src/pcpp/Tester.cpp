
#include "pcpp/Tester.hpp"
#include "pcp/Aliases.hpp"
#include "three_color/three_color.hpp"

namespace pcpp {

Tester::Tester(pcp::PoweringDomain u, pcp::PoweringDomain v, constraint::PoweringConstraint constraint) : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
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

const int THREE_COLOR_ASSIGNMENT;

Tester::Tester(three_color::Color u, three_color::Color v) : rng(std::chrono::steady_clock::now().time_since_epoch().count()), assignment(THREE_COLOR_ASSIGNMENT) {
    std::bitset<2> ubits = three_color::color_to_bits(u);
    std::bitset<2> vbits = three_color::color_to_bits(v);

    // stores 3 bits per variable, and their products
    // hardcode products
    assignment[0] = ubits[0];
    assignment[1] = ubits[1];
    assignment[2] = ubits[0] * ubits[1];
    assignment[3] = vbits[0];
    assignment[4] = vbits[1];
    assignment[5] = vbits[0] * vbits[0];
    assignment[6] = ubits[0] * vbits[1];
    assignment[7] = ubits[0] * vbits[2];
    assignment[8] = ubits[1] * vbits[0];
    assignment[9] = ubits[1] * vbits[1];
    assignment[10] = ubits[1] * vbits[2];
    assignment[11] = ubits[2] * vbits[0];
    assignment[12] = ubits[2] * vbits[1];
    assignment[13] = ubits[2] * vbits[2];
    assignment[14] = 1;

    hadamard = Hadamard(assignment);

    constraint_matrix = std::vector<std::vector<pcp::BitDomain>>(1, std::vector<pcp::BitDomain>(THREE_COLOR_ASSIGNMENT, 1));
}

pcp::BitPCP Tester::buildBitPCP(int linearity_sampling_coeff) {
    std::vector<pcp::BitDomain> variables = assignment;

    variables.reserve(variables.size() + hadamard.getCode().size() + 1);

    variables.insert(variables.end(), hadamard.getCode().begin(), hadamard.getCode().end());

    variables.push_back(0); // add extra variable for linearity test to verify 0

    pcp::BitPCP result = std::move(variables);

    size_t offset = assignment.size();

    // Add constraints between original variables and their Hadamard code position
    for (size_t i = 0; i < offset; ++i) {
        size_t i_vec = 1 << i;
        result.add_constraint(i, offset + i_vec, constraint::BitConstraint::EQUAL);
    }

    // Add constraints from the constraint matrix by sampling

    std::uniform_int_distribution<size_t> dist(0, 1 << constraint_matrix.size());
    for (size_t _ = 0; _ < linearity_sampling_coeff; ++_) {
        pcp::Variable position = 0;
        size_t sample = dist(rng);
        for (size_t j = 0; j < constraint_matrix.size(); ++j) {
            if ((sample >> j) & 1) {
                // include this constraint in the linear combination
                for (size_t k = 0; k < constraint_matrix[j].size(); ++k) {
                    if (constraint_matrix[j][k]) {
                        position ^= (1 << k);
                    }
                }
            }
        }
        result.add_constraint(offset + position, result.get_size() - 1, constraint::BitConstraint::EQUAL);
    }

    result.clean();

    return result;
}

}