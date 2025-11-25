#include <vector>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"
#include "pcpp/Tester.hpp"
#include "pcpp/Hadamard.hpp"


namespace core {

pcp::BitPCP reduce_alphabet(const pcp::PoweringPCP &pcp, int linearity_sampling_coeff) {
    pcp::BitPCP result(0);
    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        auto constraints = pcp.get_constraints(u);
        for (const auto &[v, constraint] : constraints) {
            pcpp::Tester tester(
                pcp.get_variables(u), 
                pcp.get_variables(v), 
                constraint
            );
            result = std::move(pcp::merge_BitPCP(std::move(result), std::move(tester.buildBitPCP(linearity_sampling_coeff))));
        }
    }
    return result;
}

}