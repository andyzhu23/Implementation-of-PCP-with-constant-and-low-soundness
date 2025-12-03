
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