
#ifndef SIMPLEPCP_HPP
#define SIMPLEPCP_HPP

#include <functional>
#include <vector>

#include "constraint/BinaryConstraint.hpp"
#include "Aliases.hpp"

namespace pcp {

class SimplePCP {
public:
    SimplePCP(size_t size);

    SimplePCP(const std::vector<SimpleDomain> &variables);

    SimplePCP(std::vector<SimpleDomain> &&variables);
    
    // Member functions
    size_t get_size() const;

    SimpleDomain get_variable(Variable var) const;

    void set_variable(Variable var, SimpleDomain value);

    const std::vector<std::pair<Variable, constraint::BinaryConstraint>>& get_constraints(Variable var) const;

    const std::vector<std::pair<Variable, Index>>& get_constraints_indices(Variable var) const;

    const std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>>& get_constraints_list() const;

    void add_constraint(Variable var, Variable other_var, constraint::BinaryConstraint constraint);

private:
    size_t size;
    std::vector<SimpleDomain> variables;
    // adjacent list representation of constraints used for graph traversal
    std::vector<std::vector<std::pair<Variable, constraint::BinaryConstraint>>> constraints;
    // edge list representation of constraints used for fast access to query constraints
    std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>> constraints_list;
    // storing indexes of constraint in neighbor's list for quick access
    std::vector<std::vector<std::pair<Variable, Index>>> constraint_indices;
    mutable std::vector<bool> visited; // For BFS
};

}

#endif