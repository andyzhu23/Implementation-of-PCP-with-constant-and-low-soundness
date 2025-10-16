#include <queue>

#include "BitPCP.hpp"

namespace pcp {

// BitPCP class implementation
// Constructors
BitPCP::BitPCP(size_t size) : size(size), bits(size, false), constraints(size), visited(size, false) {}

BitPCP::BitPCP(const std::vector<bool> &bits) : size(bits.size()), bits(bits), constraints(bits.size()), visited(size, false) {}

BitPCP::BitPCP(std::vector<bool> &&bits) : size(bits.size()), bits(std::move(bits)), constraints(size), visited(size, false) {}

BitPCP::BitPCP(const std::vector<bool> &bits, const std::vector<std::vector<std::pair<int, BitConstraint>>> &constraints)
    : size(bits.size()), bits(bits), constraints(constraints), visited(size, false) {}

BitPCP::BitPCP(std::vector<bool> &&bits, const std::vector<std::vector<std::pair<int, BitConstraint>>> &constraints)
    : size(bits.size()), bits(std::move(bits)), constraints(constraints), visited(size, false) {}

BitPCP::BitPCP(const std::vector<bool> &bits, std::vector<std::vector<std::pair<int, BitConstraint>>> &&constraints)
    : size(bits.size()), bits(bits), constraints(std::move(constraints)), visited(size, false) {}

BitPCP::BitPCP(std::vector<bool> &&bits, std::vector<std::vector<std::pair<int, BitConstraint>>> &&constraints)
    : size(bits.size()), bits(std::move(bits)), constraints(std::move(constraints)), visited(size, false) {}

// Member functions
size_t BitPCP::get_size() const { return size; }

bool BitPCP::get_bit(int index) const { return bits[index]; }

void BitPCP::set_bit(int index, bool value) { bits[index] = value; }

const std::vector<std::pair<int, BitConstraint>>& BitPCP::get_constraints(int index) const { return constraints[index]; }

void BitPCP::add_constraint(int index, int other_index, BitConstraint constraint) {
    constraints[index].emplace_back(other_index, constraint);
    constraints[other_index].emplace_back(index, constraint);
}

std::vector<int> BitPCP::get_neighbors(int index, int radius) const {
    std::vector<int> neighbors;
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
    // Reset visited for future calls
    for (int node : neighbors) {
        visited[node] = false;
    }

    return neighbors;
}

}