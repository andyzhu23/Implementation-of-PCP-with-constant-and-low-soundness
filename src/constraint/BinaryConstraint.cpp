#include "pcp/BinaryDomain.hpp"
#include "constraint/BinaryConstraint.hpp"

namespace constraint {

bool evaluateBinaryConstraint(BinaryConstraint constraint, pcp::BinaryDomain x, pcp::BinaryDomain y) {
    switch (constraint) {
        case BinaryConstraint::ANY:
            return true;
        case BinaryConstraint::EQUAL:
            return x == y;
        case BinaryConstraint::NOTEQUAL:
            return x != y;
        case BinaryConstraint::FIRST_BIT_EQUAL:
            return x[0] == y[0];
        case BinaryConstraint::SECOND_BIT_EQUAL:
            return x[1] == y[1];
        case BinaryConstraint::THIRD_BIT_EQUAL:
            return x[2] == y[2];
        default:
            return false; // should not reach here
    }
}

}