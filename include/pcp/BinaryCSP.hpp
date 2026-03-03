#ifndef BinaryCSP_HPP
#define BinaryCSP_HPP

#include <functional>
#include <vector>
#include <iostream>

#include "constraint/BinaryConstraint.hpp"
#include "Aliases.hpp"
#include "pcp/BinaryDomain.hpp"

namespace pcp {

class BinaryCSP {
public:
    BinaryCSP();

    BinaryCSP(size_t size);

    BinaryCSP(const std::vector<BinaryDomain> &variables);

    BinaryCSP(std::vector<BinaryDomain> &&variables);

    BinaryCSP(std::vector<BinaryDomain> &&variables,
        const std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>> &constraints_list);

    BinaryCSP(std::vector<BinaryDomain> &&variables,
        std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>> &&constraints_list);
    
    // Member functions
    size_t get_size() const;

    BinaryDomain get_variable(Variable var) const;

    void set_variable(Variable var, BinaryDomain value);

    void add_variable(BinaryDomain value);

    const std::vector<std::pair<Variable, constraint::BinaryConstraint>>& get_constraints(Variable var) const;

    const std::vector<std::pair<Variable, Index>>& get_constraints_indices(Variable var) const;

    const std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>>& get_constraints_list() const;

    void add_constraint(Variable var, Variable other_var, constraint::BinaryConstraint constraint);

    // BFS to get all neighbors within a certain radius
    std::vector<Variable> get_neighbors(Variable var, int radius) const;

    // Get a BinaryCSP consisting of the neighboring variables and constraints within a certain radius
    BinaryCSP get_neighboring_pcp(Variable var, int radius) const;

    // Build a sub-BinaryCSP from a list of variables
    BinaryCSP build_sub_pcp(const std::vector<Variable> &neighbors) const;

    // getting rid of variables with no constraints running through it
    void clean();

    // Overload output stream operator for easy printing for debugging
    friend std::ostream& operator<<(std::ostream &os, const BinaryCSP &BinaryCSP);

private:
    size_t size;
    std::vector<BinaryDomain> variables;
    // adjacent list representation of constraints used for graph traversal
    std::vector<std::vector<std::pair<Variable, constraint::BinaryConstraint>>> constraints;
    // edge list representation of constraints used for fast access to query constraints
    std::vector<std::tuple<Variable, Variable, constraint::BinaryConstraint>> constraints_list;
    // storing indexes of constraint in neighbor's list for quick access
    std::vector<std::vector<std::pair<Variable, Index>>> constraint_indices;
};

BinaryCSP merge_BinaryCSPs(const std::vector<BinaryCSP> &pcps);
BinaryCSP merge_BinaryCSPs(std::vector<BinaryCSP> &&pcps);

}

#endif