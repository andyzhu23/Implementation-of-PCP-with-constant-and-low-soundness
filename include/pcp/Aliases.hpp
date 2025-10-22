#ifndef PCP_HPP
#define PCP_HPP

#include <vector>

namespace pcp {

// type used to represent variable index
using Variable = size_t;
// type used to represent constraint index
using Index = size_t;
// type used to represent domain of values in SimplePCP
using SimpleDomain = int;
// type used to represent domain of values in PoweringPCP
using PoweringDomain = std::vector<SimpleDomain>;

}

#endif