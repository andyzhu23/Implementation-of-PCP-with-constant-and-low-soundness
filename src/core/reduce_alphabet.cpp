#include <vector>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"
#include "pcpp/Tester.hpp"
#include "pcpp/Hadamard.hpp"

namespace core {

pcp::BitPCP reduce_alphabet(const pcp::PoweringPCP &pcp) {
    pcp::BitPCP result(0);
    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        auto constraints = pcp.get_constraints(u);
        for (const auto &[v, constraint] : constraints) {

            pcpp::Tester tester(
                pcp.get_variables(u), 
                pcp.get_variables(v), 
                constraint
            );
            pcp::BitPCP tmp = tester.buildBitPCP();
            // reduce unnecessary variables
            tmp.clean();
            result = pcp::merge_BitPCPs(std::vector<pcp::BitPCP>{ std::move(result), std::move(tmp) });
        }
    }
    return result;
}

}