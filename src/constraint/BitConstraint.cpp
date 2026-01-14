
#include "constraint/BitConstraint.hpp"
#include "pcp/BitDomain.hpp"

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
        case BitConstraint::FIRST_BIT_EQUAL_SECOND_BIT:
            return x[0] == y[1];
        case BitConstraint::SECOND_BIT_EQUAL_FIRST_BIT:
            return x[1] == y[0];
        case BitConstraint::FIRST_BIT_EQUAL_THIRD_BIT:
            return x[0] == y[2];
        case BitConstraint::THIRD_BIT_EQUAL_FIRST_BIT:
            return x[2] == y[0];
        case BitConstraint::SECOND_BIT_EQUAL_THIRD_BIT:
            return x[1] == y[2];
        case BitConstraint::THIRD_BIT_EQUAL_SECOND_BIT:
            return x[2] == y[1];
        case BitConstraint::FIRST_BIT_NOTEQUAL:
            return x[0] != y[0];
        case BitConstraint::SECOND_BIT_NOTEQUAL:
            return x[1] != y[1];
        case BitConstraint::THIRD_BIT_NOTEQUAL:
            return x[2] != y[2];
        case BitConstraint::FIRST_BIT_NOTEQUAL_SECOND_BIT:
            return x[0] != y[1];
        case BitConstraint::SECOND_BIT_NOTEQUAL_FIRST_BIT:
            return x[1] != y[0];
        case BitConstraint::FIRST_BIT_NOTEQUAL_THIRD_BIT:
            return x[0] != y[2];
        case BitConstraint::THIRD_BIT_NOTEQUAL_FIRST_BIT:
            return x[2] != y[0];
        case BitConstraint::SECOND_BIT_NOTEQUAL_THIRD_BIT:
            return x[1] != y[2];
        case BitConstraint::THIRD_BIT_NOTEQUAL_SECOND_BIT:
            return x[2] != y[1];
        default:
            return false; // should not reach here
    }
}

}