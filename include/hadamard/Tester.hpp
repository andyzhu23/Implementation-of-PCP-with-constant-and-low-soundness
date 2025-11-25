/*
 * Author: Andy Zhu
 * @date    2025-11-22 10:27:26
 * @version 1.0.0
 */

#ifndef TESTER_HPP
#define TESTER_HPP

#include <vector>

#include "pcp/PoweringPCP.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "hadamard/Hadamard.hpp"

namespace pcpp {

class Tester {
public:
    Tester(pcp::PoweringDomain u, pcp::PoweringDomain v, constraint::PoweringConstraint constraint);
private:
    std::vector<bool> values;
    Hadamard hadamard;
};

}

#endif