#ifndef FINITE_FIELD_MONOMIAL_HPP
#define FINITE_FIELD_MONOMIAL_HPP

#include <vector>

#include "finite_field/FiniteFieldElement.hpp"

namespace finite_field {

class Monomial {
public:
    // Default constructor for Monomial
    Monomial();

    // Constructor for Monomial
    Monomial(FiniteFieldElement coefficient, const std::vector<size_t> &variable_exp);

    // Move constructor for Monomial
    Monomial(FiniteFieldElement coefficient, std::vector<size_t> &&variable_exp);

    // Get the coefficient of the monomial
    FiniteFieldElement getCoefficient() const;

    // Get the exponents of the variables
    const std::vector<size_t> &getVariableExps() const;

    // Get the exponent of the variable at the given index
    const size_t getExp(size_t index) const;

    // Evaluate the monomial at the given variable values
    FiniteFieldElement evaluate(const std::vector<FiniteFieldElement> &variable_values) const ;

private:
    // Coefficient of the monomial
    FiniteFieldElement coefficient;
    // Exponents of the variables in the monomial
    std::vector<size_t> variable_exp;
};

}

#endif