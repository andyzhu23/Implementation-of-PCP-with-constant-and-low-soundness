#include <chrono>
#include <random>
#include <unordered_map>

#include "constants.hpp"
#include "pcpp/HadamardPCPP/HadamardTester.hpp"
#include "Aliases.hpp"
#include "three_color/ThreeColor.hpp"
#include "util/disjoint_set_union.hpp"

void merge_variables(
    std::vector<three_color::Color> colors,
    pcp::BitPCP &pcp, 
    const std::vector<std::vector<std::pair<size_t, int>>> &occuring_location,
    const std::vector<pcp::BitPCP> &reduced_pcps
) {

    util::disjoint_set_union dsu(pcp.get_size());

    for (three_color::Node u = 0; u < colors.size(); ++u) {
        const auto &locations = occuring_location[u];
        for (size_t i = 1; i < locations.size(); ++i) {
            size_t x = locations[i].first;
            size_t y = locations[i - 1].first;
            switch (locations[i].second) {
                case 0:  // position 0: u's bits at (var 0, 1st bit) and (var 0, 3rd bit)
                    switch (locations[i - 1].second) {
                        case 0:  // both at position 0
                            dsu.merge(x, y);
                            dsu.merge(x + 2, y + 2);
                            break;
                        case 1:  // current at position 0, previous at position 1
                            dsu.merge(x, y + 1);
                            dsu.merge(x + 2, y + 3);
                            break;
                    }
                    break;
                case 1:  // position 1: v's bits at (var 0, 2nd bit) and (var 1, 1st bit)
                    switch (locations[i - 1].second) {
                        case 0:  // current at position 1, previous at position 0
                            dsu.merge(x + 1, y);
                            dsu.merge(x + 3, y + 2);
                            break;
                        case 1:  // both at position 1
                            dsu.merge(x + 1, y + 1);
                            dsu.merge(x + 3, y + 3);
                            break;
                    }
                    break;
            }
        }
    }
    
    std::unordered_map<pcp::Variable, pcp::Variable> representative_map;
    size_t new_size = 0;
    for (pcp::Variable i = 0; i < pcp.get_size(); ++i) {
        if (dsu.find(i) == i) {
            representative_map[i] = new_size++;
        }
    }

    pcp::BitPCP new_bitpcp(new_size);

    for (pcp::Variable i = 0; i < pcp.get_size(); ++i) {
        if (dsu.find(i) == i) {
            new_bitpcp.set_variable(
                representative_map[i],
                pcp.get_variable(i)
            );
        }
    }

    for (const auto &[u, v, c] : pcp.get_constraints_list()) {
        pcp::Variable new_u = representative_map[dsu.find(u)];
        pcp::Variable new_v = representative_map[dsu.find(v)];
        new_bitpcp.add_constraint(new_u, new_v, c);
    }
    pcp = std::move(new_bitpcp);
}

namespace pcpp {

const int THREE_COLOR_ASSIGNMENT = 2 + 2 + 2 * 2 + 1; // 2 bits per color, 4 products, and 1 negation bit

HadamardTester::HadamardTester() {}

HadamardTester::HadamardTester(three_color::Color u, three_color::Color v) {
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

// Construct HadamardTester from a BitPCP
HadamardTester::HadamardTester(const pcp::BitPCP &pcp) {
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        three_csp.add_variable(pcp.get_variable(i)[0]);
        three_csp.add_variable(pcp.get_variable(i)[1]);
        three_csp.add_variable(pcp.get_variable(i)[2]);
        // record index shift for encoded binary variables
        // shift second and third bits' index to point to first bit's index
        if (pcp.get_variable(i).get_domain_type() == three_csp::Constraint::ENCODED_BINARY) {
            binary_index_shift[three_csp.size() - 2] = three_csp.size() - 3;
            binary_index_shift[three_csp.size() - 1] = three_csp.size() - 3;
        }
#ifdef ENFORCE_CONSISTENCY
        if (pcp.get_variable(i).get_domain_type() != three_csp::Constraint::ANY) {
            three_csp.add_ternary_constraint(
                i * 3, 
                i * 3 + 1, 
                i * 3 + 2, 
                pcp.get_variable(i).get_domain_type()
            );
        }
#endif
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
                case constraint::BitConstraint::ANY:
                    // impossible case
                    break;
            }
            constraint_matrix.push_back(std::move(row));
        }
    }
}

void HadamardTester::create_tester(const three_color::ThreeColor &tc) { 
    std::vector<pcp::BitPCP> edge_pcps;
    size_t variable_count = 0;
    std::vector<std::vector<std::pair<size_t, int>>> occuring_locations(tc.get_colors().size());
    for (three_color::Node u = 0; u < tc.get_colors().size(); ++u) {
        for (three_color::Node v : tc.get_adj_list()[u]) if (v > u) {
            HadamardTester tester(tc.get_colors()[u], tc.get_colors()[v]);
            pcp::BitPCP tmp = tester.buildBitPCP();
            // reduce unnecessary variables
            edge_pcps.push_back(std::move(tmp));
            occuring_locations[u].emplace_back(variable_count, 0);
            occuring_locations[v].emplace_back(variable_count, 1);
            variable_count += edge_pcps.back().get_size();
        }
    }
    auto result = pcp::merge_BitPCPs(edge_pcps);
    merge_variables(tc.get_colors(), result, occuring_locations, edge_pcps);
    result.clean();
    *this = std::move(HadamardTester(result));
} 

void HadamardTester::create_tester(const pcp::BitPCP &powering_pcp) { 
    *this = std::move(HadamardTester(powering_pcp));
}

pcp::BitPCP HadamardTester::buildBitPCP() {

    size_t original_size = three_csp.size();

    std::unordered_map<std::vector<bool>, size_t> used_positions;
    std::vector<std::vector<bool>> used_positions_list;

    auto add_position = [&](const std::vector<bool>& position) {
        if (used_positions.find(position) == used_positions.end()) {
            used_positions[position] = three_csp.size();
            three_csp.add_variable(hadamard.query(position));
            used_positions_list.push_back(position);
        }
    };

    add_position(std::vector<bool>(original_size, 0));

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
        add_position(position);
    }

    size_t zero_pos = used_positions[std::vector<bool>(original_size, 0)];
    for (const auto &[pos, idx] : used_positions) {
        three_csp.add_binary_constraint(zero_pos, idx, constraint::BitConstraint::EQUAL);
    }

    std::uniform_int_distribution<size_t> dist(0, original_size - 1);
    // add linearity test to verify hadamard code encodes original variables correctly
    for (size_t _ = 0; _ < constants::CONSISTENCY_TEST_REPETITION; ++_) {
        size_t idx = dist(constants::RANDOM_SEED);
        if (binary_index_shift.find(idx) != binary_index_shift.end()) {
            idx = binary_index_shift[idx];
        }

        std::uniform_int_distribution<size_t> position_dist(0, used_positions_list.size() - 1);
        std::vector<bool> position1 = used_positions_list[position_dist(constants::RANDOM_SEED)];

        std::vector<bool> position2 = position1;
        position2[idx] = !position2[idx];

        add_position(position1);
        add_position(position2);

        three_csp.add_ternary_constraint(
            idx,
            used_positions[position1],
            used_positions[position2],
            three_csp::Constraint::SUM
        );
    }

    // add linearity test to ensure hadamard code is linear
    for (size_t _ = 0; _ < constants::LINEARITY_TEST_REPETITION; ++_) {

        // only add positions that are already used for better coverage
        std::uniform_int_distribution<size_t> position_dist(0, used_positions_list.size() - 1);

        std::vector<bool> position1 = used_positions_list[position_dist(constants::RANDOM_SEED)];

        std::vector<bool> position2 = used_positions_list[position_dist(constants::RANDOM_SEED)];

        // position3 is the xor sum of position1 and position2
        std::vector<bool> position3(original_size, 0);
        for (size_t j = 0; j < original_size; ++j) {
            position3[j] = position1[j] ^ position2[j];
        }

        // ensure all three positions are added to three_csp
        add_position(position1);
        add_position(position2);
        add_position(position3);

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