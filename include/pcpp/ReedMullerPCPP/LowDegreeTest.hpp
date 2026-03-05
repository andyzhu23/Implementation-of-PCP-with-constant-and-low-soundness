#ifndef LOW_DEGREE_TEST_HPP
#define LOW_DEGREE_TEST_HPP

#include "finite_field/Polynomial.hpp"
#include "finite_field/UnivariatePolynomial.hpp"
#include "pcpp/ReedMullerPCPP/ReedMuller.hpp"

namespace pcpp {

class LowDegreeTest {
public:
    LowDegreeTest(ReedMuller code, int degree_bound);

    // Returns the univariate polynomial obtained from evaluating the Reed-Muller code on the random line defined by slope and intercept, and applying INTT to the evaluations at roots of unity
    const finite_field::UnivariatePolynomial& getUnivariatePolynomial() const;
    
    // Verifies that the univariate polynomial correctly represents the evaluation of the Reed-Muller code on the random line defined by slope and intercept
    bool verifyPolynomial() const;

private:
    ReedMuller code;
    int degree_bound;
    std::vector<finite_field::FiniteFieldElement> slope;
    std::vector<finite_field::FiniteFieldElement> intercept;
    finite_field::UnivariatePolynomial univariate_poly; // Coefficients of the univariate polynomial after INTT evaluation
};

}

#endif