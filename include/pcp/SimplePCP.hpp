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

class SimplePCP {
public:
    SimplePCP(size_t size);

    SimplePCP(const std::vector<int> &variables);

    SimplePCP(std::vector<int> &&variables);
    
    // Member functions
    size_t get_size() const;

    int get_variable(int index) const;

    void set_variable(int index, int value);

    const std::vector<std::pair<int, constraint::BinaryConstraint>>& get_constraints(int index) const;

    const std::vector<std::pair<int, int>>& get_constraints_indices(int index) const;

    void add_constraint(int index, int other_index, constraint::BinaryConstraint constraint);

    // BFS to get all neighbors within a certain radius
    std::vector<int> get_neighbors(int index, int radius) const;

private:
    size_t size;
    std::vector<int> variables;
    std::vector<std::vector<std::pair<int, constraint::BinaryConstraint>>> constraints;
    // storing indexes of constraint in neighbor's list for quick access
    std::vector<std::vector<std::pair<int, int>>> constraint_indices;
    mutable std::vector<bool> visited; // For BFS
};

}

#endif