#include <cstdint>
#include <queue>
#include <stdexcept>
#include <unordered_set>

#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"
#include "util.hpp"

namespace pcp {

// BitPCP class implementation
// Constructors
BitPCP::BitPCP() : BitPCP(0) {}

BitPCP::BitPCP(size_t size)
 : size(size), 
   variables(size), 
   constraints(size), 
   constraint_indices(size) {}

BitPCP::BitPCP(const std::vector<BitDomain> &variables)
 : size(variables.size()), 
   variables(variables), 
   constraints(variables.size()), 
   constraint_indices(variables.size()) {}

BitPCP::BitPCP(std::vector<BitDomain> &&variables)
 : size(variables.size()), 
   variables(std::move(variables)), 
   constraints(size), 
   constraint_indices(size) {}

// Member functions
size_t BitPCP::get_size() const { return size; }

BitDomain BitPCP::get_variable(Variable var) const { return variables[var]; }

void BitPCP::set_variable(Variable var, BitDomain value) { variables[var] = value; }

void BitPCP::add_variable(BitDomain value) {
    variables.push_back(value);
    constraints.emplace_back();
    constraint_indices.emplace_back();
    ++size;
}

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
    switch (constraint) {
        case constraint::BitConstraint::ANY:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::EQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::NOTEQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::FIRST_BIT_EQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::SECOND_BIT_EQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::THIRD_BIT_EQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::FIRST_BIT_EQUAL_SECOND_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::SECOND_BIT_EQUAL_FIRST_BIT);
            break;
        case constraint::BitConstraint::SECOND_BIT_EQUAL_FIRST_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::FIRST_BIT_EQUAL_SECOND_BIT);
            break;
        case constraint::BitConstraint::FIRST_BIT_EQUAL_THIRD_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::THIRD_BIT_EQUAL_FIRST_BIT);
            break;
        case constraint::BitConstraint::THIRD_BIT_EQUAL_FIRST_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::FIRST_BIT_EQUAL_THIRD_BIT);
            break;
        case constraint::BitConstraint::SECOND_BIT_EQUAL_THIRD_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::THIRD_BIT_EQUAL_SECOND_BIT);
            break;
        case constraint::BitConstraint::THIRD_BIT_EQUAL_SECOND_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::SECOND_BIT_EQUAL_THIRD_BIT);
            break;
        case constraint::BitConstraint::FIRST_BIT_NOTEQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::SECOND_BIT_NOTEQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::THIRD_BIT_NOTEQUAL:
            constraints[other_var].emplace_back(var, constraint);
            break;
        case constraint::BitConstraint::FIRST_BIT_NOTEQUAL_SECOND_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::SECOND_BIT_NOTEQUAL_FIRST_BIT);
            break;
        case constraint::BitConstraint::SECOND_BIT_NOTEQUAL_FIRST_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::FIRST_BIT_NOTEQUAL_SECOND_BIT);
            break;
        case constraint::BitConstraint::FIRST_BIT_NOTEQUAL_THIRD_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::THIRD_BIT_NOTEQUAL_FIRST_BIT);
            break;
        case constraint::BitConstraint::THIRD_BIT_NOTEQUAL_FIRST_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::FIRST_BIT_NOTEQUAL_THIRD_BIT);
            break;
        case constraint::BitConstraint::SECOND_BIT_NOTEQUAL_THIRD_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::THIRD_BIT_NOTEQUAL_SECOND_BIT);
            break;
        case constraint::BitConstraint::THIRD_BIT_NOTEQUAL_SECOND_BIT:
            constraints[other_var].emplace_back(var, constraint::BitConstraint::SECOND_BIT_NOTEQUAL_THIRD_BIT);
            break;
    }

    constraint_indices[var].emplace_back(other_var, constraints[other_var].size() - 1);
    constraint_indices[other_var].emplace_back(var, constraints[var].size() - 1);
    constraints_list.emplace_back(var, other_var, constraint);
}
std::vector<Variable> BitPCP::get_neighbors(Variable var, int radius) const {
    std::vector<Variable> neighbors;
    std::unordered_set<Variable> visited;
    std::queue<std::pair<Variable, int>> q; // pair of (node, current_depth)
    q.emplace(var, 0);
    visited.insert(var);

    while (!q.empty()) {
        auto [current, depth] = q.front();
        q.pop();
        neighbors.push_back(current);
        if (depth < radius) {
            for (const auto &[neighbor, _] : constraints[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.emplace(neighbor, depth + 1);
                }
            }
        }
    }

    return neighbors;
}

BitPCP BitPCP::get_neighboring_pcp(Variable var, int radius) const {
    std::vector<Variable> neighbors = get_neighbors(var, radius);
    return build_sub_pcp(neighbors);
}

BitPCP BitPCP::build_sub_pcp(std::vector<Variable> &neighbors) const {
    std::unordered_map<Variable, Variable> index_map; // original index to new index
    std::unordered_set<Variable> neighbor_set(neighbors.begin(), neighbors.end());

    BitPCP neighboring_pcp(neighbors.size());
    // Copy variables
    for (size_t i = 0; i < neighbors.size(); ++i) {
        neighboring_pcp.set_variable(static_cast<Variable>(i), get_variable(neighbors[i]));
        index_map[neighbors[i]] = static_cast<Variable>(i);
    }
    // Copy constraints using constraints_list to ensure we get the original constraint types

    for (Variable u : neighbors) {
        for (const auto &[other_var, constraint] : get_constraints(u)) {
            if (neighbor_set.find(other_var) != neighbor_set.end() && u <= other_var) {
                neighboring_pcp.add_constraint(
                    index_map[u],
                    index_map[other_var],
                    constraint
                );
            }
        }
    }

    return neighboring_pcp;
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

BitPCP merge_BitPCPs(const std::vector<BitPCP> &pcps) {
    std::vector<BitDomain> merged_variables;
    size_t total_size = 0;
    for (const auto& pcp : pcps) {
        total_size += pcp.get_size();
    }
    merged_variables.reserve(total_size);
    for (const auto& pcp : pcps) {
        for (pcp::Variable i = 0; i < pcp.get_size(); ++i) {
            merged_variables.push_back(pcp.get_variable(i));
        }
    }
    BitPCP result = std::move(merged_variables);
    size_t offset = 0;
    for (const auto& pcp : pcps) {
        for (const auto& [u, v, c] : pcp.get_constraints_list()) {
            if (c != constraint::BitConstraint::ANY) {
                result.add_constraint(u + offset, v + offset, c);
            }
        }
        offset += pcp.get_size();
    }

    return result;
}

BitPCP merge_BitPCPs(std::vector<BitPCP> &&pcps) {
    std::vector<BitDomain> merged_variables;
    size_t total_size = 0;
    for (const auto& pcp : pcps) {
        total_size += pcp.get_size();
    }
    merged_variables.reserve(total_size);
    for (const auto& pcp : pcps) {
        for (pcp::Variable i = 0; i < pcp.get_size(); ++i) {
            merged_variables.push_back(pcp.get_variable(i));
        }
    }
    BitPCP result = std::move(merged_variables);
    size_t offset = 0;
    for (const auto& pcp : pcps) {
        for (const auto& [u, v, c] : pcp.get_constraints_list()) {
            if (c != constraint::BitConstraint::ANY) {
                result.add_constraint(u + offset, v + offset, c);
            }
        }
        offset += pcp.get_size();
    }

    return result;
}

}

