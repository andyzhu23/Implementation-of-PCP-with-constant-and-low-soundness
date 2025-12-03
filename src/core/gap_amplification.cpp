#include <cmath>

#include "core/core.hpp"
#include "constants.hpp"
#include "pcpp/Tester.hpp"

#include <iostream>

namespace core {

pcp::BitPCP gap_amplification(pcp::BitPCP pcp) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = std::move(reduce_degree(pcp, constants::DEGREE));

    std::vector<pcp::BitPCP> reduced_pcps;

    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        std::cout << "Reducing alphabet for variable " << u << " / " << pcp.get_size() << std::endl;
        pcp::BitPCP powering_u = pcp.get_neighboring_pcp(u, constants::POWERING_RADIUS);
        pcpp::Tester tester(powering_u);
        pcp::BitPCP reduced_pcp = tester.buildBitPCP();
        reduced_pcp.clean();
        reduced_pcps.push_back(std::move(reduced_pcp));
    }
    pcp = pcp::merge_BitPCPs(reduced_pcps);

    return pcp;
}

}