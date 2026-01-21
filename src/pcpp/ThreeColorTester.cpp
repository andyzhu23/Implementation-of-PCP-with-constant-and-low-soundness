#include <chrono>
#include <random>
#include <unordered_map>
#include <stdexcept>

#include "constants.hpp"
#include "pcpp/ThreeColorTester.hpp"
#include "Aliases.hpp"
#include "three_color/ThreeColor.hpp"

namespace pcpp {

ThreeColorTester::ThreeColorTester(three_color::Color u, three_color::Color v) {
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
#ifdef ENFORCE_CONSISTENCY
    three_csp.add_ternary_constraint(0, 2, 4, three_csp::Constraint::PRODUCT);
    three_csp.add_ternary_constraint(0, 3, 5, three_csp::Constraint::PRODUCT);
    three_csp.add_ternary_constraint(1, 2, 6, three_csp::Constraint::PRODUCT);
    three_csp.add_ternary_constraint(1, 3, 7, three_csp::Constraint::PRODUCT);
    three_csp.add_binary_constraint(0, 8, constraint::BitConstraint::NOTEQUAL);
#endif
    hadamard = Hadamard(three_csp.get_assignment());

    constraint_matrix = std::vector<std::vector<bool>>{
        // the local assignment u and v have different colors
        {0, 1, 1, 1, 1, 1, 1, 1, 1},
    };
}

pcp::BitPCP ThreeColorTester::buildBitPCP() {

    size_t original_size = three_csp.size();

    std::unordered_map<std::vector<bool>, size_t> used_positions;

    used_positions[std::vector<bool>(original_size, 0)] = three_csp.size();
    three_csp.add_variable(hadamard.query(std::vector<bool>(original_size, 0)));
    

    std::uniform_int_distribution<pcp::Variable> bernoulli(0, 1);

    // add constraints from original constraint matrix

    for (size_t _ = 0; _ < constants::CONSTRAINT_COMBINATION_REPETITION; ++_) {
        // random sample
        std::vector<bool> sample(constraint_matrix.size());
        for (size_t i = 0; i < constraint_matrix.size(); ++i) {
            sample[i] = bernoulli(constants::RANDOM_SEED);
        }

        std::vector<bool> position(original_size);
        for (size_t j = 0; j < constraint_matrix.size(); ++j) {
            if (sample[j]) {
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
        three_csp.add_binary_constraint(zero_pos, idx, constraint::BitConstraint::EQUAL);
    }

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

    // add linearity test to ensure hadamard code is linear
    for (size_t _ = 0; _ < constants::LINEARITY_TEST_REPETITION; ++_) {
        std::vector<bool> position1(original_size, 0);
        for (size_t j = 0; j < original_size; ++j) {
            position1[j] = bernoulli(constants::RANDOM_SEED);
        }
        std::vector<bool> position2(original_size, 0);
        for (size_t j = 0; j < original_size; ++j) {
            position2[j] = bernoulli(constants::RANDOM_SEED);
        }

        // compute their differences to find third position to query
        std::vector<bool> *minuend, *subtrahend;
        if (position1 < position2) {
            minuend = &position2;
            subtrahend = &position1;
        } else {
            minuend = &position1;
            subtrahend = &position2;
        }

        // set up position3 = minuend - subtrahend
        std::vector<bool> position3 = *subtrahend;
        // convert position3 to 2's complement
        // step 1: bitwise negation
        position3.flip();
        // step 2: add 1 to carry
        bool carry = 1;
        for (size_t j = 0; j < original_size; ++j) {
            bool tmp = position3[j];
            position3[j] = position3[j] ^ minuend->at(j) ^ carry;
            carry = minuend->at(j) & tmp;
        }

        // ensure all three positions are added to three_csp
        if (used_positions.find(position1) == used_positions.end()) {
            used_positions[position1] = three_csp.size();
            three_csp.add_variable(hadamard.query(position1));
        }

        if (used_positions.find(position2) == used_positions.end()) {
            used_positions[position2] = three_csp.size();
            three_csp.add_variable(hadamard.query(position2));
        }

        if (used_positions.find(position3) == used_positions.end()) {
            used_positions[position3] = three_csp.size();
            three_csp.add_variable(hadamard.query(position3));
        }

        three_csp.add_ternary_constraint(
            used_positions[position1],
            used_positions[position2],
            used_positions[position3],
            three_csp::Constraint::SUM
        );
    }

    return three_csp.toBitPCP();
}

}