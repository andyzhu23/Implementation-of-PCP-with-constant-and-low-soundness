
#ifndef BinaryConstraint_HPP
#define BinaryConstraint_HPP

#include <functional>

#include "Aliases.hpp"
#include "pcp/BinaryDomain.hpp"

namespace constraint {

// Evaluate the BinaryConstraint on two BinaryDomain values
bool evaluateBinaryConstraint(BinaryConstraint constraint, pcp::BinaryDomain x, pcp::BinaryDomain y);

}

#endif