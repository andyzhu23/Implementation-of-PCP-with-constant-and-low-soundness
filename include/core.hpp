#ifndef CORE_HPP
#define CORE_HPP

#include "BitPCP.hpp"
#include "PoweringPCP.hpp"

namespace core {

pcp::PoweringPCP powering_operation(const pcp::BitPCP &pcp, int radius);

}

#endif
