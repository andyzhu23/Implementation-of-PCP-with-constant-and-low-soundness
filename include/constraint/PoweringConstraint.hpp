
/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#ifndef POWERINGCONSTRAINT_HPP
#define POWERINGCONSTRAINT_HPP

#include <vector>

#include "pcp/Aliases.hpp"
#include "constraint/BitConstraint.hpp"
#include "pcp/SimplePCP.hpp"

namespace constraint {

class PoweringConstraint {
public:
    PoweringConstraint(size_t size);

    void add_constraint(pcp::Variable var, pcp::Variable other_var, BitConstraint constraint);

    const std::vector<std::pair<pcp::Variable, BitConstraint>>& get_constraints(pcp::Variable var) const;

private:
    std::vector<std::vector<std::pair<pcp::Variable, BitConstraint>>> constraints;
};

}

#endif