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

#include "pcp/BitPCP.hpp"
#include "pcp/PoweringPCP.hpp"
#include "constraint/BitConstraint.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "pcpp/Hadamard.hpp"
#include "three_color/three_color.hpp"
#include "ThreeCSP/ThreeCSP.hpp"

namespace pcpp {

class Tester {
public:
    Tester(pcp::PoweringDomain u, pcp::PoweringDomain v, constraint::PoweringConstraint constraint);

    Tester(three_color::Color u, three_color::Color v);

    Tester(pcp::BitPCP powering_pcp);

    // Build a BitPCP from the constraint matrix and hadamard code
    pcp::BitPCP buildBitPCP();

private:
    three_csp::ThreeCSP three_csp;
    Hadamard hadamard;
    std::vector<std::vector<bool>> constraint_matrix;
    // Expected parity (in GF(2)) for each constraint row under the canonical assignment
    std::vector<bool> constraint_parities;
};

}

#endif