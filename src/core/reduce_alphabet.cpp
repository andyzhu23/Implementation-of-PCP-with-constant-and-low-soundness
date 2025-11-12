#include "core/core.hpp"

namespace core {

pcp::SimplePCP reduce_alphabet(const pcp::PoweringPCP &pcp) {
    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
        auto constraints = pcp.get_constraints(u);
        for (const auto &[v, constraint] : constraints) {
            
        }
    }
}

}