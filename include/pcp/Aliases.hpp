
#ifndef PCP_HPP
#define PCP_HPP

#include <bitset>
#include <vector>

namespace pcp {

// size of SimpleDomain, e.g., if SimpleDomainSize = 3, then each value has 3 bits
const size_t SimpleDomainSize = 3; 

// type used to represent variable index
using Variable = __uint128_t;
// type used to represent constraint index
using Index = size_t;
// type used to represent domain of values in SimplePCP
using SimpleDomain = std::bitset<SimpleDomainSize>;
// type used to represent domain of values in BitPCP
using BitDomain = bool;
// type used to represent domain of values in PoweringPCP
using PoweringDomain = std::vector<BitDomain>;

}

#endif