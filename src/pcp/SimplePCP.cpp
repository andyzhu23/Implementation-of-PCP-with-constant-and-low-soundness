
#include <cstdint>
#include <queue>
#include <stdexcept>

#include "pcp/SimplePCP.hpp"
#include "util.hpp"

namespace pcp {

// SimplePCP class implementation
// Constructors
SimplePCP::SimplePCP(size_t size)
 : size(size), 
   variables(size, false), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

SimplePCP::SimplePCP(const std::vector<SimpleDomain> &variables)
 : size(variables.size()), 
   variables(variables), 
   constraints(variables.size()), 
   constraint_indices(variables.size()), 
   visited(size, false) {}

SimplePCP::SimplePCP(std::vector<SimpleDomain> &&variables)
 : size(variables.size()), 
   variables(std::move(variables)), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

// Member functions
size_t SimplePCP::get_size() const { return size; }

SimpleDomain SimplePCP::get_variable(Variable var) const { return variables[var]; }

void SimplePCP::set_variable(Variable var, SimpleDomain value) { variables[var] = value; }

const std::vector<std::pair<Variable, constraint::BinaryConstraint>>& SimplePCP::get_constraints(Variable var) const { 
    return constraints[var]; 
}

const std::vector<std::pair<Variable, Index>>& SimplePCP::get_constraints_indices(Variable var) const { 
    return constraint_indices[var]; 
}

const std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>>& SimplePCP::get_constraints_list() const {
    return constraints_list;
}

void SimplePCP::add_constraint(Variable var, Variable other_var, constraint::BinaryConstraint constraint) {
    if (var < 0 || var >= static_cast<Variable>(size) 
        || other_var < 0 || other_var >= static_cast<Variable>(size)) {
        throw std::out_of_range("SimplePCP::add_constraint: index out of range");
    }
    constraints[var].emplace_back(other_var, constraint);
    constraints[other_var].emplace_back(var, constraint);
    constraint_indices[var].emplace_back(other_var, constraints[other_var].size() - 1);
    constraint_indices[other_var].emplace_back(var, constraints[var].size() - 1);
    constraints_list.emplace_back(var, other_var, constraint);
}

}