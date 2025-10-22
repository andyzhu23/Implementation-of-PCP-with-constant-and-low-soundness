/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#ifndef BITPCP_HPP
#define BITPCP_HPP

#include <vector>

namespace pcp {

enum class BinaryConstraint {
    UNDEFINED, // only used in unit tests
    EQUAL,
    NOT_EQUAL,
    ANY,
    NONE
};

class PCP {
public:
    PCP(size_t size);

    PCP(const std::vector<int> &variables);

    PCP(std::vector<int> &&variables);
    
    // Member functions
    size_t get_size() const;

    int get_variable(int index) const;

    void set_variable(int index, int value);

    const std::vector<std::pair<int, BinaryConstraint>>& get_constraints(int index) const;

    const std::vector<std::pair<int, int>>& get_constraints_indices(int index) const;

    void add_constraint(int index, int other_index, BinaryConstraint constraint);

    // BFS to get all neighbors within a certain radius
    std::vector<int> get_neighbors(int index, int radius) const;

private:
    size_t size;
    std::vector<int> variables;
    std::vector<std::vector<std::pair<int, BinaryConstraint>>> constraints;
    // storing indexes of constraint in neighbor's list for quick access
    std::vector<std::vector<std::pair<int, int>>> constraint_indices;
    mutable std::vector<bool> visited; // For BFS
};

}

#endif