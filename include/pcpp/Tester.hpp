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
#include "constraint/PoweringConstraint.hpp"
#include "pcpp/Hadamard.hpp"

namespace pcpp {

class Tester {
public:
    Tester(pcp::PoweringDomain u, pcp::PoweringDomain v, constraint::PoweringConstraint constraint);

    // Build a BitPCP from the constraint matrix and hadamard code
    pcp::BitPCP buildBitPCP(int linearity_sampling_coeff);

private:
    
    std::mt19937 rng; 
    std::vector<pcp::BitDomain> assignment;
    Hadamard hadamard;
    std::vector<std::vector<pcp::BitDomain>> constraint_matrix;
};

}

#endif