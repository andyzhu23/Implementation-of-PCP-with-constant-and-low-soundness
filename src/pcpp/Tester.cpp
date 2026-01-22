#include <chrono>
#include <random>
#include <unordered_map>
#include <stdexcept>

#include <iostream>

#include "constants.hpp"
#include "pcpp/Tester.hpp"
#include "Aliases.hpp"
#include "three_color/ThreeColor.hpp"

namespace pcpp {

// Construct Tester from a BitPCP
Tester::Tester(const pcp::BitPCP &pcp) : pcp(pcp) {
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        if (pcp.get_variable(i).get_domain_type() == three_csp::Constraint::ENCODED_BINARY) {
            three_csp.add_variable(pcp.get_variable(i)[0]);
            three_csp_to_pcp_index[three_csp.size() - 1] = {i, 0};

            pcp_to_three_csp_index[i] = three_csp.size() - 1;
        } else {
            // each variable in three_csp is represented by 3 bits in BitPCP
            three_csp.add_variable(pcp.get_variable(i)[0]);
            three_csp_to_pcp_index[three_csp.size() - 1] = {i, 0};

            three_csp.add_variable(pcp.get_variable(i)[1]);
            three_csp_to_pcp_index[three_csp.size() - 1] = {i, 1};
            
            three_csp.add_variable(pcp.get_variable(i)[2]);
            three_csp_to_pcp_index[three_csp.size() - 1] = {i, 2};

            pcp_to_three_csp_index[i] = three_csp.size() - 3;
        }
    }

    pcp_in_three_csp_size = three_csp.size();

    three_csp.add_variable(0); // negation bits
    three_csp.add_variable(1); // negation bits


    three_csp.add_binary_constraint(three_csp.size() - 2, three_csp.size() - 1, constraint::BitConstraint::NOTEQUAL);

    hadamard = Hadamard(three_csp.get_assignment());

    // Build constraint matrix from BitPCP constraints
    for (const auto &[var1, var2, bit_constraint] : pcp.get_constraints_list()) {
        if (bit_constraint != constraint::BitConstraint::ANY) {
            std::vector<bool> row(three_csp.size());
            switch (bit_constraint) {
                case constraint::BitConstraint::EQUAL:
                    if (pcp.get_variable(var1).get_domain_type() != three_csp::Constraint::ENCODED_BINARY && pcp.get_variable(var2).get_domain_type() != three_csp::Constraint::ENCODED_BINARY) {
                        // both are non ENCODED_BINARY
                        std::uniform_int_distribution<size_t> dist(0, 2);
                        size_t offset = dist(constants::RANDOM_SEED);
                        row[pcp_to_three_csp_index.at(var1) + offset] = 1;
                        row[pcp_to_three_csp_index.at(var2) + offset] = 1;
                    } else if (pcp.get_variable(var1).get_domain_type() != three_csp::Constraint::ENCODED_BINARY || pcp.get_variable(var2).get_domain_type() != three_csp::Constraint::ENCODED_BINARY) {
                        throw std::invalid_argument("EQUAL constraint between variables with non ENCODED_BINARY domain types");
                    } else {
                        // both are ENCODED_BINARY
                        row[pcp_to_three_csp_index.at(var1)] = 1;
                        row[pcp_to_three_csp_index.at(var2)] = 1;
                    }
                    break;
                case constraint::BitConstraint::NOTEQUAL:
                    if (pcp.get_variable(var1).get_domain_type() != three_csp::Constraint::ENCODED_BINARY || pcp.get_variable(var2).get_domain_type() != three_csp::Constraint::ENCODED_BINARY) {
                        throw std::invalid_argument("NOTEQUAL constraint between variables with non ENCODED_BINARY domain types");
                    }
                    row[pcp_to_three_csp_index.at(var1)] = 1;
                    row[pcp_to_three_csp_index.at(var2)] = 1;
                    row[row.size() - 2] = 1;
                    row.back() = 1; // constant 1 for inequality
                    break;
                case constraint::BitConstraint::FIRST_BIT_EQUAL:
                    row[pcp_to_three_csp_index.at(var1)] = 1;
                    row[pcp_to_three_csp_index.at(var2)] = 1;
                    break;
                case constraint::BitConstraint::SECOND_BIT_EQUAL:
                    // it might be the case where the variable is encoded in 2 bits only, in which case there is no need for offset
                    if (pcp.get_variable(var1).get_domain_type() == three_csp::Constraint::ENCODED_BINARY) {
                        row[pcp_to_three_csp_index.at(var1)] = 1;
                    } else {
                        row[pcp_to_three_csp_index.at(var1) + 1] = 1;
                    }
                    if (pcp.get_variable(var2).get_domain_type() == three_csp::Constraint::ENCODED_BINARY) {
                        row[pcp_to_three_csp_index.at(var2)] = 1;
                    } else {
                        row[pcp_to_three_csp_index.at(var2) + 1] = 1;
                    }
                    break;
                case constraint::BitConstraint::THIRD_BIT_EQUAL:
                    // it might be the case where the variable is encoded in 2 bits only, in which case there is no need for offset
                    if (pcp.get_variable(var1).get_domain_type() == three_csp::Constraint::ENCODED_BINARY) {
                        row[pcp_to_three_csp_index.at(var1)] = 1;
                    } else {
                        row[pcp_to_three_csp_index.at(var1) + 2] = 1;
                    }
                    if (pcp.get_variable(var2).get_domain_type() == three_csp::Constraint::ENCODED_BINARY) {
                        row[pcp_to_three_csp_index.at(var2)] = 1;
                    } else {
                        row[pcp_to_three_csp_index.at(var2) + 2] = 1;
                    }
                    break;
                default:
                    break;
            }
            constraint_matrix.push_back(std::move(row));
        }
    }
}

pcp::BitPCP Tester::buildBitPCP() {

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
        if (idx != zero_pos) {
            three_csp.add_binary_constraint(zero_pos, idx, constraint::BitConstraint::EQUAL);
        }
    }

    std::uniform_int_distribution<size_t> dist(0, original_size - 1);
    // add linearity test to verify hadamard code encodes original variables correctly
    for (size_t _ = 0; _ < constants::CONSISTENCY_TEST_REPETITION; ++_) {
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

    return three_csp.toBitPCP(pcp, three_csp_to_pcp_index, pcp_in_three_csp_size);
}

}