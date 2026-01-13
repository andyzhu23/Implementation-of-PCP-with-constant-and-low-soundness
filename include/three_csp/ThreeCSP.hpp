#ifndef THREECSP_HPP
#define THREECSP_HPP

#include <vector>

#include "pcp/BitPCP.hpp"
#include "constraint/BitConstraint.hpp"

namespace three_csp {

using Domain = bool;
using Variable = size_t;

enum class Constraint {
    PRODUCT,
    SUM,
};

class ThreeCSP {
public:
    void add_ternary_constraint(size_t var1, size_t var2, size_t var3, Constraint constraint);

    void add_binary_constraint(size_t var1, size_t var2, constraint::BitConstraint constraint);

    void add_variable(Domain value);

    const std::vector<Domain>& get_assignment() const;

    Domain get_variable(size_t index) const;

    size_t size() const;

    pcp::BitPCP toBitPCP() const;

private:
    std::vector<Domain> assignment;
    std::vector<std::tuple<size_t, size_t, size_t, Constraint>> ternary_constraints;
    std::vector<std::tuple<size_t, size_t, constraint::BitConstraint>> binary_constraints;
};

}

#endif