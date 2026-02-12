/*
 * Author: Andy Zhu
 * @date    2025-11-22 10:27:26
 * @version 1.0.0
 */

#ifndef HADAMARDTESTER_HPP
#define HADAMARDTESTER_HPP

#include <chrono>
#include <vector>
#include <random>
#include <unordered_map>

#include "pcp/BitPCP.hpp"
#include "constraint/BitConstraint.hpp"
#include "pcpp/HadamardPCPP/Hadamard.hpp"
#include "pcpp/Tester.hpp"
#include "three_color/ThreeColor.hpp"
#include "three_csp/ThreeCSP.hpp"

namespace pcpp {

class HadamardTester : public Tester {
public:

    HadamardTester();

    HadamardTester(three_color::Color u, three_color::Color v);

    HadamardTester(const pcp::BitPCP &powering_pcp);

    void create_tester(three_color::Color u, three_color::Color v); 
    
    void create_tester(const pcp::BitPCP &powering_pcp);

    // Build a BitPCP from the constraint matrix and hadamard code
    pcp::BitPCP buildBitPCP();

private:
    three_csp::ThreeCSP three_csp;
    Hadamard hadamard;
    std::vector<std::vector<bool>> constraint_matrix;
    // Expected parity (in GF(2)) for each constraint row under the canonical assignment
    std::vector<bool> constraint_parities;
    std::unordered_map<size_t, size_t> binary_index_shift;
};

}

#endif