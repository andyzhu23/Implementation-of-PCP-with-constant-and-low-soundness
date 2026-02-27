#ifndef FINITE_FIELD_POLYNOMIAL_HPP
#define FINITE_FIELD_POLYNOMIAL_HPP

#include <vector>

#include "finite_field/FiniteFieldElement.hpp"

namespace finite_field {

class UnivariatePolynomial {
public:
    UnivariatePolynomial();

    UnivariatePolynomial(size_t degree);

    UnivariatePolynomial(const std::vector<FiniteFieldElement> &coefficients);

    UnivariatePolynomial(std::vector<FiniteFieldElement> &&coefficients);

    size_t getDegree() const;

    FiniteFieldElement operator[](size_t index) const;

    FiniteFieldElement evaluate(const FiniteFieldElement &x) const;

private:
    size_t degree;
    std::vector<FiniteFieldElement> coefficients;
};

}

#endif