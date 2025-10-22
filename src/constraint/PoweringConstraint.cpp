/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#include <vector>

#include "pcp/Aliases.hpp"
#include "constraint/PoweringConstraint.hpp"
#include "pcp/SimplePCP.hpp"

namespace constraint {

// PoweringConstraint class implementation
// Constructor
PoweringConstraint::PoweringConstraint(size_t size) : constraints(size) {}

// Member functions
void PoweringConstraint::add_constraint(pcp::Variable var, pcp::Variable other_var, BinaryConstraint constraint) {
    constraints[var].emplace_back(other_var, constraint);
}

const std::vector<std::pair<pcp::Variable, BinaryConstraint>>& PoweringConstraint::get_constraints(pcp::Variable var) const {
    return constraints[var];
}

}
