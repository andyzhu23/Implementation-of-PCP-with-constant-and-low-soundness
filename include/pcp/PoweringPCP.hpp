/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#ifndef POWERINGPCP_HPP
#define POWERINGPCP_HPP

#include <vector>

#include "SimplePCP.hpp"
#include "constraint/BinaryConstraint.hpp"
#include "constraint/PoweringConstraint.hpp"

namespace pcp {

class PoweringPCP {
public:
    // Constructors
    PoweringPCP(size_t size);

    PoweringPCP(size_t size, const std::vector<std::vector<int>> &variables);

    PoweringPCP(size_t size, std::vector<std::vector<int>> &&variables);

    PoweringPCP(const std::vector<std::vector<int>> &variables,
                 const std::vector<std::vector<std::pair<int, constraint::PoweringConstraint>>> &constraints);

    PoweringPCP(std::vector<std::vector<int>> &&variables,
                 const std::vector<std::vector<std::pair<int, constraint::PoweringConstraint>>> &constraints);

    PoweringPCP(const std::vector<std::vector<int>> &variables,
                 std::vector<std::vector<std::pair<int, constraint::PoweringConstraint>>> &&constraints);

    PoweringPCP(std::vector<std::vector<int>> &&variables,
                 std::vector<std::vector<std::pair<int, constraint::PoweringConstraint>>> &&constraints);
    
    // Member functions
    size_t get_size() const;

    const std::vector<int>& get_variables(int index) const;

    void set_variables(int index, const std::vector<int> &vars);
    
    void set_variables(int index, std::vector<int> &&vars);

    const std::vector<std::pair<int, constraint::PoweringConstraint>>& get_constraints(int index) const;

    void add_constraint(int index, int other_index, const constraint::PoweringConstraint &constraint);
    
    void add_constraint(int index, int other_index, constraint::PoweringConstraint &&constraint);
    
private:
    size_t size;
    std::vector<std::vector<int>> variables;
    std::vector<std::vector<std::pair<int, constraint::PoweringConstraint>>> constraints;
};

}

#endif