/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#ifndef POWERINGPCP_HPP
#define POWERINGPCP_HPP

#include <vector>

#include "Aliases.hpp"
#include "SimplePCP.hpp"
#include "constraint/BinaryConstraint.hpp"
#include "constraint/PoweringConstraint.hpp"

namespace pcp {

class PoweringPCP {
public:
    // Constructors
    PoweringPCP(size_t size);

    PoweringPCP(size_t size, const std::vector<PoweringDomain> &variables);

    PoweringPCP(size_t size, std::vector<PoweringDomain> &&variables);

    PoweringPCP(const std::vector<PoweringDomain> &variables,
                 const std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &constraints);

    PoweringPCP(std::vector<PoweringDomain> &&variables,
                 const std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &constraints);

    PoweringPCP(const std::vector<PoweringDomain> &variables,
                 std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &&constraints);

    PoweringPCP(std::vector<PoweringDomain> &&variables,
                 std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> &&constraints);
    
    // Member functions
    size_t get_size() const;

    const PoweringDomain& get_variables(Variable var) const;

    void set_variables(Variable var, const PoweringDomain &vars);
    
    void set_variables(Variable var, PoweringDomain &&vars);

    const std::vector<std::pair<Variable, constraint::PoweringConstraint>>& get_constraints(Variable var) const;

    void add_constraint(Variable var, Variable other_var, const constraint::PoweringConstraint &constraint);
    
    void add_constraint(Variable var, Variable other_var, constraint::PoweringConstraint &&constraint);
    
private:
    size_t size;
    std::vector<PoweringDomain> variables;
    std::vector<std::vector<std::pair<Variable, constraint::PoweringConstraint>>> constraints;
};

}

#endif