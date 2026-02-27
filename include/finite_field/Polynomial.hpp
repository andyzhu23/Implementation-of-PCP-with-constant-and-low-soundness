#ifndef FINITE_FIELD_POLYNOMIAL_HPP
#define FINITE_FIELD_POLYNOMIAL_HPP

#include <vector>

#include "finite_field/Monomial.hpp"

namespace finite_field {

class Polynomial {
public:
    Polynomial();

    Polynomial(const std::vector<Monomial> &terms);

    Polynomial(std::vector<Monomial> &&terms);

    void addTerm(const Monomial &term);

    void addTerm(Monomial &&term);

    FiniteFieldElement evaluate(const std::vector<FiniteFieldElement> &variable_values) const;

private:
    std::vector<Monomial> terms;
};

}

#endif