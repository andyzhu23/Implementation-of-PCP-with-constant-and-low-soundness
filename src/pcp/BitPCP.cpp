/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#include <cstdint>
#include <queue>

#include "pcp/BitPCP.hpp"
#include "util.hpp"

namespace pcp {

// BitPCP class implementation
// Constructors
BitPCP::BitPCP(size_t size)
 : size(size), 
   bits(size, false), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

BitPCP::BitPCP(const std::vector<bool> &bits)
 : size(bits.size()), 
   bits(bits), 
   constraints(bits.size()), 
   constraint_indices(bits.size()), 
   visited(size, false) {}

BitPCP::BitPCP(std::vector<bool> &&bits)
 : size(bits.size()), 
   bits(std::move(bits)), 
   constraints(size), 
   constraint_indices(size), 
   visited(size, false) {}

// Member functions
size_t BitPCP::get_size() const { return size; }

bool BitPCP::get_bit(int index) const { return bits[index]; }

void BitPCP::set_bit(int index, bool value) { bits[index] = value; }

const std::vector<std::pair<int, BitConstraint>>& BitPCP::get_constraints(int index) const { 
    return constraints[index]; 
}

const std::vector<std::pair<int, int>>& BitPCP::get_constraints_indices(int index) const { 
    return constraint_indices[index]; 
}

void BitPCP::add_constraint(int index, int other_index, BitConstraint constraint) {
    if (index < 0 || index >= static_cast<int>(size) 
        || other_index < 0 || other_index >= static_cast<int>(size)) {
        throw std::invalid_argument("BitPCP::add_constraint: index out of range");
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