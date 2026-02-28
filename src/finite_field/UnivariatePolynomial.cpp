#include "finite_field/UnivariatePolynomial.hpp"

namespace finite_field {

UnivariatePolynomial::UnivariatePolynomial() : degree(0) {
    coefficients[0] = FiniteFieldElement(0);
}

UnivariatePolynomial::UnivariatePolynomial(size_t degree) : degree(degree), coefficients() {}

UnivariatePolynomial::UnivariatePolynomial(const std::vector<FiniteFieldElement> &coefficients)
    : degree(coefficients.size() - 1) {
    for (size_t i = 0; i < coefficients.size(); ++i) {
        this->coefficients[i] = coefficients[i];
    }
}

UnivariatePolynomial::UnivariatePolynomial(const std::map<size_t, FiniteFieldElement> &coefficients)
    : degree(coefficients.size() - 1), coefficients(coefficients) {}

UnivariatePolynomial::UnivariatePolynomial(std::map<size_t, FiniteFieldElement> &&coefficients)
    : degree(coefficients.size() - 1), coefficients(std::move(coefficients)) {}

size_t UnivariatePolynomial::getDegree() const {
    return degree;
}

FiniteFieldElement UnivariatePolynomial::operator[](size_t index) const {
    if (coefficients.find(index) == coefficients.end()) {
        return FiniteFieldElement(0);
    }
    return coefficients.at(index);
}

FiniteFieldElement UnivariatePolynomial::evaluate(const FiniteFieldElement &x) const {
    FiniteFieldElement result(0);
    FiniteFieldElement power_of_x(1);
    for (size_t i = 0; i <= degree; ++i) {
        if (coefficients.find(i) != coefficients.end()) {
            result += coefficients.at(i) * power_of_x;
        }
        power_of_x *= x;
    }
    return result;
}

}