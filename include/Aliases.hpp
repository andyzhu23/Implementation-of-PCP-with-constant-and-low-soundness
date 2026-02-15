
#ifndef PCP_HPP
#define PCP_HPP

#include <bitset>
#include <vector>

namespace pcp {

// size of BitDomain, e.g., if BitDomainSize = 3, then each value has 3 bits
const size_t BitDomainSize = 3;

// type used to represent variable index
using Variable = __uint128_t;
// type used to represent constraint index
using Index = size_t;

}

namespace constraint {

enum class BitConstraint {
    ANY,
    EQUAL,
    NOTEQUAL, 
    FIRST_BIT_EQUAL,
    SECOND_BIT_EQUAL,
    THIRD_BIT_EQUAL
};

}

namespace three_csp {

enum class Constraint {
    ANY,
    PRODUCT,
    SUM,
    ENCODED_BINARY,
    ONE_HOT_COLOR,
};

}

#endif