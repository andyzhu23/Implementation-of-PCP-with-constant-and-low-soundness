#include <stdexcept>

#include "finite_field/Monomial.hpp"

namespace finite_field {

Monomial::Monomial() : coefficient(0), variable_exp() {}

Monomial::Monomial(FiniteFieldElement coefficient, const std::vector<size_t> &variable_exp)
    : coefficient(coefficient), variable_exp(variable_exp) {}

Monomial::Monomial(FiniteFieldElement coefficient, std::vector<size_t> &&variable_exp)
    : coefficient(coefficient), variable_exp(std::move(variable_exp)) {}

FiniteFieldElement Monomial::getCoefficient() const {
    return coefficient;
}

const std::vector<size_t> &Monomial::getVariableExps() const {
    return variable_exp;
}

const size_t Monomial::getExp(size_t index) const {
    return variable_exp[index];
}

FiniteFieldElement Monomial::evaluate(const std::vector<FiniteFieldElement> &variable_values) const {
    if (variable_values.size() < variable_exp.size()) {
        throw std::invalid_argument("Number of variable values must be at least the number of variable exponents.");
    }
    FiniteFieldElement result = coefficient;
    for (size_t i = 0; i < variable_exp.size(); ++i) {
        FiniteFieldElement term = 1;
        for (size_t j = 0; j < variable_exp[i]; ++j) {
            term *= variable_values[i];
        }
        result *= term;
    }
    return result;
}

}