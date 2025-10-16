#ifndef CORE_HPP
#define CORE_HPP

#include "BitPCP.hpp"
#include "PoweringPCP.hpp"

namespace core {

// Powering operation on a BitPCP to produce a PoweringPCP from Dinur's PCP proof
pcp::PoweringPCP powering_operation(const pcp::BitPCP &pcp, int radius);

}

#endif
