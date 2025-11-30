#include <cstdint>
#include <queue>
#include <stdexcept>

#include "pcp/BitPCP.hpp"
#include "util.hpp"

namespace pcp {

// BitPCP class implementation
// Constructors
BitPCP::BitPCP(size_t size)
 : size(size), 
   variables(size, false), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

BitPCP::BitPCP(const std::vector<BitDomain> &variables)
 : size(variables.size()), 
   variables(variables), 
   constraints(variables.size()), 
   constraint_indices(variables.size()), 
   visited(size, false) {}

BitPCP::BitPCP(std::vector<BitDomain> &&variables)
 : size(variables.size()), 
   variables(std::move(variables)), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

// Member functions
size_t BitPCP::get_size() const { return size; }

BitDomain BitPCP::get_variable(Variable var) const { return variables[var]; }

void BitPCP::set_variable(Variable var, BitDomain value) { variables[var] = value; }

const std::vector<std::pair<Variable, constraint::BitConstraint>>& BitPCP::get_constraints(Variable var) const { 
    return constraints[var]; 
}

const std::vector<std::pair<Variable, Index>>& BitPCP::get_constraints_indices(Variable var) const { 
    return constraint_indices[var]; 
}

const std::vector<std::tuple<Variable, Variable, constraint::BitConstraint>>& BitPCP::get_constraints_list() const {
    return constraints_list;
}

void BitPCP::add_constraint(Variable var, Variable other_var, constraint::BitConstraint constraint) {
    if (var < 0 || var >= static_cast<Variable>(size) 
        || other_var < 0 || other_var >= static_cast<Variable>(size)) {
        throw std::out_of_range("BitPCP::add_constraint: index out of range");
    }
    constraints[var].emplace_back(other_var, constraint);
    constraints[other_var].emplace_back(var, constraint);
    constraint_indices[var].emplace_back(other_var, constraints[other_var].size() - 1);
    constraint_indices[other_var].emplace_back(var, constraints[var].size() - 1);
    constraints_list.emplace_back(var, other_var, constraint);
}
std::vector<Variable> BitPCP::get_neighbors(Variable var, int radius) const {
    std::vector<Variable> neighbors;
    util::visit_guard visit_guard(visited, neighbors); // RAII to manage visited state
    std::queue<std::pair<int, int>> q; // pair of (node, current_depth)
    q.emplace(var, 0);
    visited[var] = true;

    while (!q.empty()) {
        auto [current, depth] = q.front();
        q.pop();
        neighbors.push_back(current);
        if (depth < radius) {
            for (const auto& [neighbor, _] : constraints[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.emplace(neighbor, depth + 1);
                }
            }
        }
    }

    return neighbors;
}

void BitPCP::clean() {
    // do coordinate compression on variables
    std::vector<Variable> map(size, -1);
    std::vector<BitDomain> new_variables;
    Variable new_size = 0;
    for (Variable i = 0; i < size; ++i) {
        if (!constraints[i].empty()) {
            map[i] = new_size++;
            new_variables.push_back(variables[i]);
        }
    }
    BitPCP new_bitpcp = std::move(new_variables);

    // rebuild all constraints to new_bitpcp
    for (const auto &[u, v, c] : constraints_list) {
        new_bitpcp.add_constraint(map[u], map[v], c);
    }

    // reset current object
    *this = std::move(new_bitpcp);
}

BitPCP merge_BitPCP(BitPCP &&pcp1, BitPCP &&pcp2) {
    BitPCP result(pcp1.get_size() + pcp2.get_size());
    // Copy variables
    for (size_t i = 0; i < pcp1.get_size(); ++i) {
        result.set_variable(i, pcp1.get_variable(i));
    }
    for (size_t i = 0; i < pcp2.get_size(); ++i) {
        result.set_variable(i + pcp1.get_size(), pcp2.get_variable(i));
    }
    // Copy constraints from pcp1
    for (const auto &[u, v, constraint] : pcp1.get_constraints_list()) {
        result.add_constraint(u, v, constraint);
    }
    // Copy constraints from pcp2 with offset
    for (const auto &[u, v, constraint] : pcp2.get_constraints_list()) {
        result.add_constraint(u + pcp1.get_size(), v + pcp1.get_size(), constraint);
    }
    return result;
}

}

