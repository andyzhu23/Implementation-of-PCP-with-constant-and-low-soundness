/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#ifndef POWERINGPCP_HPP
#define POWERINGPCP_HPP

#include <vector>

#include "BitPCP.hpp"

namespace pcp {

class PoweringConstraint {
public:
    PoweringConstraint(int size);

    void add_constraint(int index, int other_index, BinaryConstraint constraint);

    const std::vector<std::pair<int, BinaryConstraint>>& get_constraints(int index) const;

private:
    std::vector<std::vector<std::pair<int, BinaryConstraint>>> constraints;
};

class PoweringPCP {
public:
    // Constructors
    PoweringPCP(size_t size);

    PoweringPCP(size_t size, const std::vector<std::vector<bool>> &variables);

    PoweringPCP(size_t size, std::vector<std::vector<bool>> &&variables);

    PoweringPCP(const std::vector<std::vector<bool>> &variables,
                 const std::vector<std::vector<std::pair<int, PoweringConstraint>>> &constraints);

    PoweringPCP(std::vector<std::vector<bool>> &&variables,
                 const std::vector<std::vector<std::pair<int, PoweringConstraint>>> &constraints);

    PoweringPCP(const std::vector<std::vector<bool>> &variables,
                 std::vector<std::vector<std::pair<int, PoweringConstraint>>> &&constraints);

    PoweringPCP(std::vector<std::vector<bool>> &&variables,
                 std::vector<std::vector<std::pair<int, PoweringConstraint>>> &&constraints);
    
    // Member functions
    size_t get_size() const;

    const std::vector<bool>& get_variables(int index) const;

    void set_variables(int index, const std::vector<bool> &vars);
    
    void set_variables(int index, std::vector<bool> &&vars);

    const std::vector<std::pair<int, PoweringConstraint>>& get_constraints(int index) const;

    void add_constraint(int index, int other_index, const PoweringConstraint &constraint);
    
    void add_constraint(int index, int other_index, PoweringConstraint &&constraint);
    
private:
    size_t size;
    std::vector<std::vector<bool>> variables;
    std::vector<std::vector<std::pair<int, PoweringConstraint>>> constraints;
};

}

#endif