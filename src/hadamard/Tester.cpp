
#include "hadamard/Tester.hpp"
#include "pcp/Aliases.hpp"

namespace pcpp {

Tester::Tester(pcp::PoweringDomain u, pcp::PoweringDomain v, constraint::PoweringConstraint constraint) {
    for (size_t i = 0; i < u.size(); ++i) {
        for (const auto[j, binary_constraint] : constraint.get_constraints(i)) {
            
        }
    }
}

}