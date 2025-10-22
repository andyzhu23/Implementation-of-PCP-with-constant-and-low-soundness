
/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#ifndef POWERINGCONSTRAINT_HPP
#define POWERINGCONSTRAINT_HPP

#include <vector>

#include "pcp/Aliases.hpp"
#include "constraint/BinaryConstraint.hpp"
#include "pcp/SimplePCP.hpp"

namespace constraint {

class PoweringConstraint {
public:
    PoweringConstraint(size_t size);

    void add_constraint(pcp::Variable var, pcp::Variable other_var, BinaryConstraint constraint);

    const std::vector<std::pair<pcp::Variable, BinaryConstraint>>& get_constraints(pcp::Variable var) const;

private:
    std::vector<std::vector<std::pair<pcp::Variable, BinaryConstraint>>> constraints;
};

}

#endif