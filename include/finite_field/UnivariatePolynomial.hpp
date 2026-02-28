#ifndef FINITE_FIELD_UNIVARIATEPOLYNOMIAL_HPP
#define FINITE_FIELD_UNIVARIATEPOLYNOMIAL_HPP

#include <map>
#include <vector>

#include "finite_field/FiniteFieldElement.hpp"

namespace finite_field {

class UnivariatePolynomial {
public:
    UnivariatePolynomial();

    UnivariatePolynomial(size_t degree);

    UnivariatePolynomial(const std::vector<FiniteFieldElement> &coefficients);

    UnivariatePolynomial(const std::map<size_t, FiniteFieldElement> &coefficients);

    UnivariatePolynomial(std::map<size_t, FiniteFieldElement> &&coefficients);

    size_t getDegree() const;

    FiniteFieldElement operator[](size_t index) const;

    FiniteFieldElement evaluate(const FiniteFieldElement &x) const;

private:
    size_t degree;
    std::map<size_t, FiniteFieldElement> coefficients;
};

}

#endif