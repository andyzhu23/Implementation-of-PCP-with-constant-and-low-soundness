/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#include <queue>
#include <vector>

#include "pcp/BitPCP.hpp"
#include "pcp/PoweringPCP.hpp"

namespace pcp {

// PoweringConstraint class implementation
// Constructor
PoweringConstraint::PoweringConstraint(int size) : constraints(size) {}

// Member functions
void PoweringConstraint::add_constraint(int index, int other_index, BitConstraint constraint) {
    constraints[index].emplace_back(other_index, constraint);
}

// Overloaded operator
const std::vector<std::pair<int, BitConstraint>>& PoweringConstraint::get_constraints(int index) const {
    return constraints[index];
}

// PoweringPCP class implementation
// Constructors
PoweringPCP::PoweringPCP(size_t size) : size(size), variables(size), constraints(size) {}

PoweringPCP::PoweringPCP(size_t size, const std::vector<std::vector<bool>> &variables)
    : size(size), variables(variables), constraints(size) {}

PoweringPCP::PoweringPCP(size_t size, std::vector<std::vector<bool>> &&variables)
    : size(size), variables(std::move(variables)), constraints(size) {}

PoweringPCP::PoweringPCP(const std::vector<std::vector<bool>> &variables,
                         const std::vector<std::vector<std::pair<int, PoweringConstraint>>> &constraints)
    : size(variables.size()), variables(variables), constraints(constraints) {}

PoweringPCP::PoweringPCP(std::vector<std::vector<bool>> &&variables,
                         const std::vector<std::vector<std::pair<int, PoweringConstraint>>> &constraints)
    : size(variables.size()), variables(std::move(variables)), constraints(constraints) {}

PoweringPCP::PoweringPCP(const std::vector<std::vector<bool>> &variables,
                         std::vector<std::vector<std::pair<int, PoweringConstraint>>> &&constraints)
    : size(variables.size()), variables(variables), constraints(std::move(constraints)) {}

PoweringPCP::PoweringPCP(std::vector<std::vector<bool>> &&variables,
                         std::vector<std::vector<std::pair<int, PoweringConstraint>>> &&constraints)
    : size(variables.size()), variables(std::move(variables)), constraints(std::move(constraints)) {}

// Member functions
size_t PoweringPCP::get_size() const { return size; }

const std::vector<bool>& PoweringPCP::get_variables(int index) const { return variables[index]; }

void PoweringPCP::set_variables(int index, const std::vector<bool> &vars) { variables[index] = vars; }

void PoweringPCP::set_variables(int index, std::vector<bool> &&vars) { variables[index] = std::move(vars); }

const std::vector<std::pair<int, PoweringConstraint>>& PoweringPCP::get_constraints(int index) const { 
    return constraints[index]; 
}

void PoweringPCP::add_constraint(int index, int other_index, const PoweringConstraint &constraint) {
    constraints[index].emplace_back(other_index, constraint);
}

void PoweringPCP::add_constraint(int index, int other_index, PoweringConstraint &&constraint) {
    constraints[index].emplace_back(other_index, constraint);
}

}