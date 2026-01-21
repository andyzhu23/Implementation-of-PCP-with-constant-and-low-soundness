/*
 * Author: Andy Zhu
 * @date    2025-11-22 10:27:26
 * @version 1.0.0
 */

#ifndef TESTER_HPP
#define TESTER_HPP

#include <chrono>
#include <vector>
#include <random>
#include <unordered_map>
#include <optional>

#include "pcp/BitPCP.hpp"
#include "constraint/BitConstraint.hpp"
#include "pcpp/Hadamard.hpp"
#include "three_color/ThreeColor.hpp"
#include "three_csp/ThreeCSP.hpp"

namespace pcpp {

class Tester {
public:

    Tester(const pcp::BitPCP &powering_pcp);

    // Build a BitPCP from the constraint matrix and hadamard code
    pcp::BitPCP buildBitPCP();

private:
    pcp::BitPCP pcp;
    three_csp::ThreeCSP three_csp;
    Hadamard hadamard;
    std::vector<std::vector<bool>> constraint_matrix;
    // Map from three_csp variable index to (pcp variable index, bit index), nullopt for encoding binary variables
    std::unordered_map<size_t, std::pair<size_t, std::optional<size_t>>> three_csp_to_pcp_index;
    std::unordered_map<size_t, size_t> pcp_to_three_csp_index;
    size_t pcp_in_three_csp_size;
};

}

#endif