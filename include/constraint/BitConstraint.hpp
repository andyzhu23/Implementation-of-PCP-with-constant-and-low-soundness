
/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#ifndef BITCONSTRAINT_HPP
#define BITCONSTRAINT_HPP

#include <functional>

#include "pcp/Aliases.hpp"

namespace constraint {

enum class BitConstraint {
    ANY,
    EQUAL,
    NOTEQUAL
};

// Evaluate the BitConstraint on two BitDomain values
bool evaluateBitConstraint(BitConstraint constraint, pcp::BitDomain x, pcp::BitDomain y);

}

#endif