#include <cstdint>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <string>

#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"

namespace pcp {

// BitPCP class implementation
// Constructors
BitPCP::BitPCP() : BitPCP(0) {}

BitPCP::BitPCP(size_t size)
 : size(size), 
   variables(size, false), 
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

BitPCP::BitPCP(std::vector<BitDomain> &&variables,
    const std::vector<std::tuple<Variable, Variable, constraint::BitConstraint>> &constraints_list)
 : BitPCP(std::move(variables)) {
    for (const auto& [u, v, c] : constraints_list) {
        add_constraint(u, v, c);
    }
}

BitPCP::BitPCP(std::vector<BitDomain> &&variables,
    std::vector<std::tuple<Variable, Variable, constraint::BitConstraint>> &&constraints_list)
 : BitPCP(std::move(variables)) {
    for (const auto& [u, v, c] : constraints_list) {
        add_constraint(u, v, c);
    }
}

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
    constraints[other_var].emplace_back(var, constraint);
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

BitPCP BitPCP::build_sub_pcp(const std::vector<Variable> &neighbors) const  {
    std::unordered_map<Variable, Variable> index_map; // original index to new index

    BitPCP neighboring_pcp(neighbors.size());
    // Copy variables
    for (size_t i = 0; i < neighbors.size(); ++i) {
        neighboring_pcp.set_variable(static_cast<Variable>(i), get_variable(neighbors[i]));
        index_map[neighbors[i]] = static_cast<Variable>(i);
    }
    // Copy constraints
    for (size_t i = 0; i < neighbors.size(); ++i) {
        Variable u = neighbors[i];
        for (const auto &[v, constraint] : constraints[u]) {
            if (index_map.find(v) != index_map.end()) {
                if (constraint != constraint::BitConstraint::ANY) {
                    neighboring_pcp.add_constraint(
                        static_cast<Variable>(i), 
                        index_map[v], 
                        constraint
                    );
                }
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

std::ostream& operator<<(std::ostream &os, const BitPCP &bitpcp) {
    os << "BitPCP with " << bitpcp.get_size() << " variables and " << bitpcp.get_constraints_list().size() << " constraints.\n";
    os << "Variables:\n";
    for (pcp::Variable i = 0; i < bitpcp.get_size(); ++i) {
        os << "Variable " << (int)i << ": ";
        const BitDomain &bd = bitpcp.get_variable(i);
        os << "[";
        for (size_t bit_idx = 0; bit_idx < 3; ++bit_idx) {
            os << bd[bit_idx];
            if (bit_idx < 2) os << ", ";
        }
        std::string domain_type_str;
        switch (bd.get_domain_type()) {
            case three_csp::Constraint::ANY:
                domain_type_str = "ANY";
                break;
            case three_csp::Constraint::PRODUCT:
                domain_type_str = "PRODUCT";
                break;
            case three_csp::Constraint::SUM:
                domain_type_str = "SUM";
                break;
            case three_csp::Constraint::ENCODED_BINARY:
                domain_type_str = "ENCODED_BINARY";
                break;
            case three_csp::Constraint::ONE_HOT_COLOR:
                domain_type_str = "ONE_HOT_COLOR";
                break;
            default:
                domain_type_str = "UNKNOWN";
                break;
        }
        os << "], Domain Type: " << domain_type_str << "\n";
    }
    os << "Constraints:\n";
    for (const auto& [u, v, c] : bitpcp.get_constraints_list()) {
        std::string constraint_type_string;
        switch (c) {
            case constraint::BitConstraint::ANY:
                constraint_type_string = "ANY";
                break;
            case constraint::BitConstraint::EQUAL:
                constraint_type_string = "EQUAL";
                break;
            case constraint::BitConstraint::NOTEQUAL:
                constraint_type_string = "NOTEQUAL";
                break;
            case constraint::BitConstraint::FIRST_BIT_EQUAL:
                constraint_type_string = "FIRST_BIT_EQUAL";
                break;
            case constraint::BitConstraint::SECOND_BIT_EQUAL:
                constraint_type_string = "SECOND_BIT_EQUAL";
                break;
            case constraint::BitConstraint::THIRD_BIT_EQUAL:
                constraint_type_string = "THIRD_BIT_EQUAL";
                break;
            default:
                constraint_type_string = "UNKNOWN";
                break;
        }
        os << "Constraint between Variable " << (int)u << " and Variable " << (int)v 
           << ": Type " << constraint_type_string << "\n";
    }
    return os;
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
            result.add_constraint(u + offset, v + offset, c);
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

