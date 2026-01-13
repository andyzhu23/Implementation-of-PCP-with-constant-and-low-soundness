
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
        case BitConstraint::FIRST_BIT_EQUAL:
            return x[0] == y[0];
        case BitConstraint::SECOND_BIT_EQUAL:
            return x[1] == y[1];
        case BitConstraint::THIRD_BIT_EQUAL:
            return x[2] == y[2];
        default:
            return false; // should not reach here
    }
}

}