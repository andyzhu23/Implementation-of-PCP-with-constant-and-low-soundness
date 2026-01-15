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

#include "constants.hpp"
#include "pcp/BitPCP.hpp"
#include "constraint/BitConstraint.hpp"
#include "pcpp/Hadamard.hpp"
#include "three_color/ThreeColor.hpp"
#include "three_csp/ThreeCSP.hpp"

namespace pcpp {

class Tester {
public:

    Tester(three_color::Color u, three_color::Color v);

    Tester(pcp::BitPCP powering_pcp);

    // Build a BitPCP from the constraint matrix and hadamard code
    pcp::BitPCP buildBitPCP(bool enforce_consistency = constants::ENFORCING_CONSISTENCY);

private:
    three_csp::ThreeCSP three_csp;
    Hadamard hadamard;
    std::vector<std::vector<bool>> constraint_matrix;
    // Expected parity (in GF(2)) for each constraint row under the canonical assignment
    std::vector<bool> constraint_parities;
};

}

#endif