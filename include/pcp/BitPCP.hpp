
#ifndef BITPCP_HPP
#define BITPCP_HPP

#include <functional>
#include <vector>

#include "constraint/BitConstraint.hpp"
#include "Aliases.hpp"

namespace pcp {

class BitPCP {
public:
    BitPCP(size_t size);

    BitPCP(const std::vector<BitDomain> &variables);

    BitPCP(std::vector<BitDomain> &&variables);
    
    // Member functions
    size_t get_size() const;

    BitDomain get_variable(Variable var) const;

    void set_variable(Variable var, BitDomain value);

    const std::vector<std::pair<Variable, constraint::BitConstraint>>& get_constraints(Variable var) const;

    const std::vector<std::pair<Variable, Index>>& get_constraints_indices(Variable var) const;

    const std::vector<std::tuple<Variable, Variable, constraint::BitConstraint>>& get_constraints_list() const;

    void add_constraint(Variable var, Variable other_var, constraint::BitConstraint constraint);

    // BFS to get all neighbors within a certain radius
    std::vector<Variable> get_neighbors(Variable var, int radius) const;

    // getting rid of variables with no constraints running through it
    void clean();

private:
    size_t size;
    std::vector<BitDomain> variables;
    // adjacent list representation of constraints used for graph traversal
    std::vector<std::vector<std::pair<Variable, constraint::BitConstraint>>> constraints;
    // edge list representation of constraints used for fast access to query constraints
    std::vector<std::tuple<Variable, Variable, constraint::BitConstraint>> constraints_list;
    // storing indexes of constraint in neighbor's list for quick access
    std::vector<std::vector<std::pair<Variable, Index>>> constraint_indices;
    mutable std::vector<BitDomain> visited; // For BFS
};

BitPCP merge_BitPCP(BitPCP &&pcp1, BitPCP &&pcp2);

}

#endif