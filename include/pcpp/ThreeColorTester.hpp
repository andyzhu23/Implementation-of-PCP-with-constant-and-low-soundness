/*
 * Author: Andy Zhu
 * @date    2025-11-22 10:27:26
 * @version 1.0.0
 */

#ifndef THREE_COLOR_TESTER_HPP
#define THREE_COLOR_TESTER_HPP

#include <chrono>
#include <vector>
#include <random>
#include <unordered_map>

#include "pcp/BitPCP.hpp"
#include "constraint/BitConstraint.hpp"
#include "pcpp/Hadamard.hpp"
#include "three_color/ThreeColor.hpp"
#include "three_csp/ThreeCSP.hpp"

namespace pcpp {

const int THREE_COLOR_ASSIGNMENT = 2 + 2 + 2 * 2 + 1; // 2 bits per color, 4 products, and 1 negation bit

class ThreeColorTester {
public:

    ThreeColorTester(three_color::Color u, three_color::Color v);

    // Build a BitPCP from the constraint matrix and hadamard code
    pcp::BitPCP buildBitPCP();

private:
    three_csp::ThreeCSP three_csp;
    Hadamard hadamard;
    std::vector<std::vector<bool>> constraint_matrix;
};

}

#endif