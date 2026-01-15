
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
    FIRST_BIT_NOTEQUAL = 6,
    SECOND_BIT_NOTEQUAL = 7,
    THIRD_BIT_NOTEQUAL = 8,
};

}

#endif