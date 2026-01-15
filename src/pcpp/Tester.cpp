#include <chrono>
#include <random>
#include <unordered_map>

#include <iostream>

#include "constants.hpp"
#include "pcpp/Tester.hpp"
#include "Aliases.hpp"
#include "three_color/ThreeColor.hpp"

namespace pcpp {

const int THREE_COLOR_ASSIGNMENT = 2 + 2 + 2 * 2 + 1; // 2 bits per color, 4 products, and 1 negation bit

Tester::Tester(three_color::Color u, three_color::Color v) {
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
    three_csp.add_variable(bit0[0]); // var 0, first bit of u
    three_csp.add_variable(bit0[1]); // var 1, first bit of v
    three_csp.add_variable(bit1[0]); // var 2, second bit of u
    three_csp.add_variable(bit1[1]); // var 3, second bit of v
    three_csp.add_variable(bit0[0] * bit1[0]); // var 4, product of first bits
    three_csp.add_variable(bit0[0] * bit1[1]); // var 5, product of u's first bit and v's second bit
    three_csp.add_variable(bit0[1] * bit1[0]); // var 6, product of v's first bit and u's second bit
    three_csp.add_variable(bit0[1] * bit1[1]); // var 7, product of second bits
    three_csp.add_variable(!bit0[0]); // var 8, negation of u's first bit

    // constraints to ensure proper coloring
    if (constants::ENFORCING_CONSISTENCY) {
        three_csp.add_ternary_constraint(0, 2, 4, three_csp::Constraint::PRODUCT);
        three_csp.add_ternary_constraint(0, 3, 5, three_csp::Constraint::PRODUCT);
        three_csp.add_ternary_constraint(1, 2, 6, three_csp::Constraint::PRODUCT);
        three_csp.add_ternary_constraint(1, 3, 7, three_csp::Constraint::PRODUCT);
        three_csp.add_binary_constraint(0, 8, constraint::BitConstraint::NOTEQUAL);
    }
    hadamard = Hadamard(three_csp.get_assignment());

    constraint_matrix = std::vector<std::vector<bool>>{
        // the local assignment u and v have different colors
        {0, 1, 1, 1, 1, 1, 1, 1, 1},
    };
}

// Construct Tester from a BitPCP
Tester::Tester(pcp::BitPCP pcp) {
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        three_csp.add_variable(pcp.get_variable(i)[0]);
        three_csp.add_variable(pcp.get_variable(i)[1]);
        three_csp.add_variable(pcp.get_variable(i)[2]);
        three_csp.add_ternary_constraint(
            i * 3,
            i * 3 + 1,
            i * 3 + 2,
            pcp.get_variable(i).get_domain_type()
        );
    }

    three_csp.add_variable(!three_csp.get_variable(0)); // negation bit for first variable

    three_csp.add_binary_constraint(0, three_csp.size() - 1, constraint::BitConstraint::NOTEQUAL);

    hadamard = Hadamard(three_csp.get_assignment());
    // Build constraint matrix from BitPCP constraints
    for (const auto &[var1, var2, bit_constraint] : pcp.get_constraints_list()) {
        if (bit_constraint != constraint::BitConstraint::ANY) {
            std::vector<bool> row(three_csp.size());
            switch (bit_constraint) {
                case constraint::BitConstraint::EQUAL:
                    row[var1 * 3] = 1;
                    row[var1 * 3 + 1] = 1;
                    row[var1 * 3 + 2] = 1;
                    row[var2 * 3] = 1;
                    row[var2 * 3 + 1] = 1;
                    row[var2 * 3 + 2] = 1;
                    break;
                case constraint::BitConstraint::FIRST_BIT_EQUAL:
                    row[var1 * 3] = 1;
                    row[var2 * 3] = 1;
                    break;
                case constraint::BitConstraint::SECOND_BIT_EQUAL:
                    row[var1 * 3 + 1] = 1;
                    row[var2 * 3 + 1] = 1;
                    break;
                case constraint::BitConstraint::THIRD_BIT_EQUAL:
                    row[var1 * 3 + 2] = 1;
                    row[var2 * 3 + 2] = 1;
                    break;
                // Note that NOTEQUAL constraint is only added between three bit encoded of 0 and 1, meaning all three bits must be different to qualify as NOTEQUAL
                case constraint::BitConstraint::NOTEQUAL:
                    row[var1 * 3] = 1;
                    row[var1 * 3 + 1] = 1;
                    row[var1 * 3 + 2] = 1;
                    row[var2 * 3] = 1;
                    row[var2 * 3 + 1] = 1;
                    row[var2 * 3 + 2] = 1;
                    row[0] = !row[0]; 
                    row.back() = 1; // negation bit
                    break;
                case constraint::BitConstraint::FIRST_BIT_NOTEQUAL:
                    row[var1 * 3] = 1;
                    row[var2 * 3] = 1;
                    row[0] = !row[0]; // negation bit
                    row.back() = 1;
                    break;
                case constraint::BitConstraint::SECOND_BIT_NOTEQUAL:
                    row[var1 * 3 + 1] = 1;
                    row[var2 * 3 + 1] = 1;
                    row[0] = !row[0]; // negation bit
                    row.back() = 1;
                    break;
                case constraint::BitConstraint::THIRD_BIT_NOTEQUAL:
                    row[var1 * 3 + 2] = 1;
                    row[var2 * 3 + 2] = 1;
                    row[0] = !row[0]; // negation bit
                    row.back() = 1;
                    break;
                case constraint::BitConstraint::ANY:
                    // impossible case
                    break;
            }
            constraint_matrix.push_back(std::move(row));
        }
    }
}

pcp::BitPCP Tester::buildBitPCP(bool enforce_consistency) {

    size_t original_size = three_csp.size();

    std::unordered_map<std::vector<bool>, size_t> used_positions;

    used_positions[std::vector<bool>(original_size, 0)] = original_size;
    three_csp.add_variable(0);

    std::uniform_int_distribution<pcp::Variable> bernoulli(0, 1);

    // add constraints from original constraint matrix

    for (size_t _ = 0; _ < constants::CONSTRAINT_COMBINATION_REPETITION; ++_) {
        // random sample
        std::vector<bool> position(original_size);

        for (size_t j = 0; j < constraint_matrix.size(); ++j) {
            if (bernoulli(constants::RANDOM_SEED)) {
                // include this constraint in the linear combination
                for (size_t k = 0; k < constraint_matrix[j].size(); ++k) {
                    position[k] = position[k] ^ constraint_matrix[j][k];
                }
            }
        }

        if (used_positions.find(position) == used_positions.end()) {
            used_positions[position] = three_csp.size();
            three_csp.add_variable(hadamard.query(position));
        }
    }

    size_t zero_pos = used_positions[std::vector<bool>(original_size, 0)];
    for (const auto &[pos, idx] : used_positions) {
        if (idx != zero_pos) {  // Skip self-edge
            three_csp.add_binary_constraint(zero_pos, idx, constraint::BitConstraint::EQUAL);
        }
    }

    if (enforce_consistency) {
        std::uniform_int_distribution<size_t> dist(0, original_size - 1);
        // add linearity test to verify hadamard code encodes original variables correctly

        for (size_t _ = 0; _ < constants::LINEARITY_TEST_REPETITION; ++_) {
            size_t idx = dist(constants::RANDOM_SEED);
            std::vector<bool> position1(original_size, 0);
            for (size_t j = 0; j < original_size; ++j) {
                position1[j] = bernoulli(constants::RANDOM_SEED);
            }

            std::vector<bool> position2 = position1;
            position2[idx] = !position2[idx];
            if (used_positions.find(position1) == used_positions.end()) {
                used_positions[position1] = three_csp.size();
                three_csp.add_variable(hadamard.query(position1));
            }

            if (used_positions.find(position2) == used_positions.end()) {
                used_positions[position2] = three_csp.size();
                three_csp.add_variable(hadamard.query(position2));
            }

            three_csp.add_ternary_constraint(
                idx,
                used_positions[position1],
                used_positions[position2],
                three_csp::Constraint::SUM
            );
        }
    }
    return three_csp.toBitPCP();
}

}