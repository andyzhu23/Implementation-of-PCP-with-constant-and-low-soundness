#include "finite_field/UnivariatePolynomial.hpp"

namespace finite_field {

UnivariatePolynomial::UnivariatePolynomial() : degree(0) {
    coefficients.push_back(FiniteFieldElement(0));
}

UnivariatePolynomial::UnivariatePolynomial(size_t degree) : degree(degree), coefficients(degree + 1) {}

UnivariatePolynomial::UnivariatePolynomial(const std::vector<FiniteFieldElement> &coefficients)
    : degree(coefficients.size() - 1), coefficients(coefficients) {
        while (this->degree > 0 && this->coefficients.back() == 0) {
            --this->degree;
            this->coefficients.pop_back();
        }
    }

UnivariatePolynomial::UnivariatePolynomial(std::vector<FiniteFieldElement> &&coefficients)
    : degree(coefficients.size() - 1), coefficients(std::move(coefficients)) {
        while (this->degree > 0 && this->coefficients.back() == 0) {
            --this->degree;
            this->coefficients.pop_back();
        }
    }   

size_t UnivariatePolynomial::getDegree() const {
    return degree;
}

FiniteFieldElement UnivariatePolynomial::operator[](size_t index) const {
    if (index >= coefficients.size()) {
        return FiniteFieldElement(0);
    }
    return coefficients.at(index);
}

FiniteFieldElement UnivariatePolynomial::evaluate(const FiniteFieldElement &x) const {
    FiniteFieldElement result(0);
    FiniteFieldElement power_of_x(1);
    for (size_t i = 0; i <= degree; ++i) {
        result += coefficients[i] * power_of_x; // coeffs[i] * x^i
        power_of_x *= x;
    }
    return result;
}

}