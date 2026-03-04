#ifndef LOW_DEGREE_TEST_HPP
#define LOW_DEGREE_TEST_HPP

#include "finite_field/Polynomial.hpp"
#include "finite_field/UnivariatePolynomial.hpp"
#include "pcpp/ReedMullerPCPP/ReedMuller.hpp"

namespace pcpp {

class LowDegreeTest {
public:
    LowDegreeTest(ReedMuller code);

private:
    ReedMuller code;
    std::vector<finite_field::FiniteFieldElement> slope;
    std::vector<finite_field::FiniteFieldElement> intercept;
};

}

#endif