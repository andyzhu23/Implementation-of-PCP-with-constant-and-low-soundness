/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#ifndef SIMPLEPCP_HPP
#define SIMPLEPCP_HPP

#include <functional>
#include <vector>

#include "constraint/BinaryConstraint.hpp"

namespace pcp {

using SimpleDomain = int;
using Variable = size_t;
using Index = size_t;

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

    void add_constraint(Variable var, Variable other_var, constraint::BinaryConstraint constraint);

    // BFS to get all neighbors within a certain radius
    std::vector<Variable> get_neighbors(Variable var, int radius) const;

private:
    size_t size;
    std::vector<SimpleDomain> variables;
    std::vector<std::vector<std::pair<Variable, constraint::BinaryConstraint>>> constraints;
    // storing indexes of constraint in neighbor's list for quick access
    std::vector<std::vector<std::pair<Variable, Index>>> constraint_indices;
    mutable std::vector<bool> visited; // For BFS
};

}

#endif