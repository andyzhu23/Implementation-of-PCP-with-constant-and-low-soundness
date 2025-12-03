
#include <vector>

#include "pcp/Aliases.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "pcp/BitPCP.hpp"

namespace constraint {

// PoweringConstraint class implementation
// Constructor
PoweringConstraint::PoweringConstraint(size_t size) : constraints(size) {}

// Member functions
void PoweringConstraint::add_constraint(pcp::Variable var, pcp::Variable other_var, BitConstraint constraint) {
    constraints[var].emplace_back(other_var, constraint);
}

const std::vector<std::pair<pcp::Variable, BitConstraint>>& PoweringConstraint::get_constraints(pcp::Variable var) const {
    return constraints[var];
}

}
