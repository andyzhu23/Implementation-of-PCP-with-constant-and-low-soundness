
#ifndef PCP_HPP
#define PCP_HPP

#include <bitset>
#include <vector>

namespace pcp {

// type used to represent variable index
using Variable = __uint128_t;
// type used to represent constraint index
using Index = size_t;

}

namespace three_csp {

using Domain = bool;
using Variable = size_t;

enum class Constraint {
    ANY,
    ENCODED_BINARY,
    PRODUCT,
    SUM
};

}

namespace constraint {

enum class BitConstraint {
    ANY = 0,
    EQUAL = 1,
    NOTEQUAL = 2, 
    FIRST_BIT_EQUAL = 3,
    SECOND_BIT_EQUAL = 4,
    THIRD_BIT_EQUAL = 5,
    FIRST_BIT_EQUAL_SECOND_BIT = 6,
    SECOND_BIT_EQUAL_FIRST_BIT = 7,
    FIRST_BIT_EQUAL_THIRD_BIT = 8,
    THIRD_BIT_EQUAL_FIRST_BIT = 9,
    SECOND_BIT_EQUAL_THIRD_BIT = 10,
    THIRD_BIT_EQUAL_SECOND_BIT = 11,
    FIRST_BIT_NOTEQUAL = 12,
    SECOND_BIT_NOTEQUAL = 13,
    THIRD_BIT_NOTEQUAL = 14,
    FIRST_BIT_NOTEQUAL_SECOND_BIT = 15,
    SECOND_BIT_NOTEQUAL_FIRST_BIT = 16,
    FIRST_BIT_NOTEQUAL_THIRD_BIT = 17,
    THIRD_BIT_NOTEQUAL_FIRST_BIT = 18,
    SECOND_BIT_NOTEQUAL_THIRD_BIT = 19,
    THIRD_BIT_NOTEQUAL_SECOND_BIT = 20
};

}

#endif