
#include <queue>
#include <stdexcept>
#include <vector>

#include "pcp/SimplePCP.hpp"
#include "pcp/PoweringPCP.hpp"

namespace pcp {

// PoweringPCP class implementation
// Constructors
PoweringPCP::PoweringPCP(size_t size) : size(size), variables(size), constraints(size) {}

PoweringPCP::PoweringPCP(size_t size, const std::vector<PoweringDomain> &variables)
    : size(size), variables(variables), constraints(size) {}

PoweringPCP::PoweringPCP(size_t size, std::vector<PoweringDomain> &&variables)
    : size(size), variables(std::move(variables)), constraints(size) {}

PoweringPCP::PoweringPCP(const std::vector<PoweringDomain> &variables,
                         const std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &constraints)
    : size(variables.size()), variables(variables), constraints(constraints) {}

PoweringPCP::PoweringPCP(std::vector<PoweringDomain> &&variables,
                         const std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &constraints)
    : size(variables.size()), variables(std::move(variables)), constraints(constraints) {}

PoweringPCP::PoweringPCP(const std::vector<PoweringDomain> &variables,
                         std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &&constraints)
    : size(variables.size()), variables(variables), constraints(std::move(constraints)) {}

PoweringPCP::PoweringPCP(std::vector<PoweringDomain> &&variables,
                         std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &&constraints)
    : size(variables.size()), variables(std::move(variables)), constraints(std::move(constraints)) {}

// Member functions
size_t PoweringPCP::get_size() const { return size; }

const PoweringDomain& PoweringPCP::get_variables(Variable var) const { return variables[var]; }

void PoweringPCP::set_variables(Variable var, const PoweringDomain &vars) { variables[var] = vars; }

void PoweringPCP::set_variables(Variable var, PoweringDomain &&vars) { variables[var] = std::move(vars); }

const std::vector<std::pair<Variable, constraint::PoweringConstraint>>& PoweringPCP::get_constraints(Variable var) const { 
    return constraints[var]; 
}

void PoweringPCP::add_constraint(Variable var, Variable other_var, const constraint::PoweringConstraint &constraint) {
    if (var < 0 || var >= static_cast<Variable>(size) 
            || other_var < 0 || other_var >= static_cast<Variable>(size)) {
        throw std::out_of_range("PoweringPCP::add_constraint: index out of range");
    }
    constraints[var].emplace_back(other_var, constraint);
}

void PoweringPCP::add_constraint(Variable var, Variable other_var, constraint::PoweringConstraint &&constraint) {
    if (var < 0 || var >= static_cast<Variable>(size) 
            || other_var < 0 || other_var >= static_cast<Variable>(size)) {
        throw std::out_of_range("PoweringPCP::add_constraint: index out of range");
    }
    constraints[var].emplace_back(other_var, std::move(constraint));
}

}