
#ifndef BITCONSTRAINT_HPP
#define BITCONSTRAINT_HPP

#include <functional>

#include "Aliases.hpp"
#include "pcp/BitDomain.hpp"

namespace constraint {

// Evaluate the BitConstraint on two BitDomain values
bool evaluateBitConstraint(BitConstraint constraint, pcp::BitDomain x, pcp::BitDomain y);

}

#endif