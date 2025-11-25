/*
 * Author: Andy Zhu
 * @date    2025-11-25 16:19:09
 * @version 1.0.0
 */

#include "constraint/BitConstraint.hpp"

namespace constraint {

bool evaluateBitConstraint(BitConstraint constraint, pcp::BitDomain x, pcp::BitDomain y) {
    switch (constraint) {
        case BitConstraint::ANY:
            return true;
        case BitConstraint::EQUAL:
            return x == y;
        case BitConstraint::NOTEQUAL:
            return x != y;
        default:
            return false; // should not reach here
    }
}

}