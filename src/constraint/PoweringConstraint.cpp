/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#include <vector>

#include <constraint/PoweringConstraint.hpp>

namespace constraint {

// PoweringConstraint class implementation
// Constructor
PoweringConstraint::PoweringConstraint(int size) : constraints(size) {}

// Member functions
void PoweringConstraint::add_constraint(int index, int other_index, BinaryConstraint constraint) {
    constraints[index].emplace_back(other_index, constraint);
}

// Overloaded operator
const std::vector<std::pair<int, BinaryConstraint>>& PoweringConstraint::get_constraints(int index) const {
    return constraints[index];
}

}
