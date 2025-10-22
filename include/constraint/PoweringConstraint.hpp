
/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#ifndef POWERINGCONSTRAINT_HPP
#define POWERINGCONSTRAINT_HPP

#include <vector>

#include <constraint/BinaryConstraint.hpp>

namespace constraint {

class PoweringConstraint {
public:
    PoweringConstraint(int size);

    void add_constraint(int index, int other_index, BinaryConstraint constraint);

    const std::vector<std::pair<int, BinaryConstraint>>& get_constraints(int index) const;

private:
    std::vector<std::vector<std::pair<int, BinaryConstraint>>> constraints;
};

}

#endif