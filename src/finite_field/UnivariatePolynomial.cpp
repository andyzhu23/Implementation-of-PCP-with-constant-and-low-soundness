#include "finite_field/UnivariatePolynomial.hpp"

namespace finite_field {

UnivariatePolynomial::UnivariatePolynomial() : degree(0), coefficients(1, FiniteFieldElement(0)) {}

UnivariatePolynomial::UnivariatePolynomial(size_t degree) : degree(degree), coefficients(degree + 1, FiniteFieldElement(0)) {}

UnivariatePolynomial::UnivariatePolynomial(const std::vector<FiniteFieldElement> &coefficients)
    : degree(coefficients.size() - 1), coefficients(coefficients) {}

UnivariatePolynomial::UnivariatePolynomial(std::vector<FiniteFieldElement> &&coefficients)
    : degree(coefficients.size() - 1), coefficients(std::move(coefficients)) {}

size_t UnivariatePolynomial::getDegree() const {
    return degree;
}

FiniteFieldElement UnivariatePolynomial::operator[](size_t index) const {
    if (index > degree) {
        return FiniteFieldElement(0);
    }
    return coefficients[index];
}

FiniteFieldElement UnivariatePolynomial::evaluate(const FiniteFieldElement &x) const {
    FiniteFieldElement result(0);
    FiniteFieldElement power_of_x(1);
    for (size_t i = 0; i <= degree; ++i) {
        result += coefficients[i] * power_of_x;
        power_of_x *= x;
    }
    return result;
}

}