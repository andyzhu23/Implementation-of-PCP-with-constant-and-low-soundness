#include "core/core.hpp"
#include "pcp/BitPCP.hpp"

namespace core {

pcp::BitPCP reduce_alphabet(const pcp::PoweringPCP &pcp) {
    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        auto constraints = pcp.get_constraints(u);
        for (const auto &[v, constraint] : constraints) {
            
        }
    }
}

}