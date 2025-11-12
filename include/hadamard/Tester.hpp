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
    std::vector<bool> original_value;
    Hadamard hadamard;
};

}

#endif