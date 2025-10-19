/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

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

BitPCP::BitPCP(const std::vector<bool> &variables)
 : size(variables.size()), 
   variables(variables), 
   constraints(variables.size()), 
   constraint_indices(variables.size()), 
   visited(size, false) {}

BitPCP::BitPCP(std::vector<bool> &&variables)
 : size(variables.size()), 
   variables(std::move(variables)), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

// Member functions
size_t BitPCP::get_size() const { return size; }

bool BitPCP::get_variable(int index) const { return variables[index]; }

void BitPCP::set_variable(int index, bool value) { variables[index] = value; }

const std::vector<std::pair<int, BinaryConstraint>>& BitPCP::get_constraints(int index) const { 
    return constraints[index]; 
}

const std::vector<std::pair<int, int>>& BitPCP::get_constraints_indices(int index) const { 
    return constraint_indices[index]; 
}

void BitPCP::add_constraint(int index, int other_index, BinaryConstraint constraint) {
    if (index < 0 || index >= static_cast<int>(size) 
        || other_index < 0 || other_index >= static_cast<int>(size)) {
        throw std::out_of_range("BitPCP::add_constraint: index out of range");
    }
    constraints[index].emplace_back(other_index, constraint);
    constraints[other_index].emplace_back(index, constraint);
    constraint_indices[index].emplace_back(other_index, constraints[other_index].size() - 1);
    constraint_indices[other_index].emplace_back(index, constraints[index].size() - 1);
}

std::vector<int> BitPCP::get_neighbors(int index, int radius) const {
    std::vector<int> neighbors;
    util::visit_guard visit_guard(visited, neighbors); // RAII to manage visited state
    std::queue<std::pair<int, int>> q; // pair of (node, current_depth)
    q.emplace(index, 0);
    visited[index] = true;

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

}