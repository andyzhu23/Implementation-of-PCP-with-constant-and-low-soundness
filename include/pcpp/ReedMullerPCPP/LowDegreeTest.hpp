#ifndef LOW_DEGREE_TEST_HPP
#define LOW_DEGREE_TEST_HPP

#include "finite_field/Polynomial.hpp"
#include "finite_field/UnivariatePolynomial.hpp"

namespace pcpp {

class LowDegreeTest {
public:
    LowDegreeTest(finite_field::Polynomial p);

private:
    finite_field::UnivariatePolynomial h;
    std::vector<finite_field::FiniteFieldElement> slope;
    std::vector<finite_field::FiniteFieldElement> intercept;
};

}

#endif