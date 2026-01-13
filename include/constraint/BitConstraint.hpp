
#ifndef BITCONSTRAINT_HPP
#define BITCONSTRAINT_HPP

#include <functional>

#include "pcp/Aliases.hpp"

namespace constraint {

enum class BitConstraint {
    ANY,
    EQUAL,
    NOTEQUAL, 
    FIRST_BIT_EQUAL,
    SECOND_BIT_EQUAL,
    THIRD_BIT_EQUAL
};

// Evaluate the BitConstraint on two BitDomain values
bool evaluateBitConstraint(BitConstraint constraint, pcp::BitDomain x, pcp::BitDomain y);

}

#endif