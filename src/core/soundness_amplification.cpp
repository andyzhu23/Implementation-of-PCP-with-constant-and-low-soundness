#include <cmath>

#include "core/core.hpp"
#include "constants.hpp"

namespace core {

pcp::BitPCP soundness_amplification(pcp::BitPCP pcp) {
    int repeat = std::log2(pcp.get_size());
    for (int i = 0; i < repeat; ++i) {
        pcp = std::move(reduce_degree(pcp, constants::DEGREE));
        pcp = std::move(to_expander(pcp, constants::EXPANDING_COEFFICIENT));
        pcp::PoweringPCP powered_pcp = powering_operation(pcp, constants::POWERING_RADIUS);
        pcp::BitPCP reduced_pcp = reduce_alphabet(powered_pcp, constants::LINEARITY_COEFFICIENT);
        pcp = std::move(reduced_pcp);
    }
    return pcp;
}

}