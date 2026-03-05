#ifndef REED_MULLER_CODE_HPP
#define REED_MULLER_CODE_HPP

#include <functional>

#include "three_csp/ThreeCSP.hpp"
#include "finite_field/Polynomial.hpp"

namespace pcpp {

using PolynomialOracle = std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)>;

class ReedMuller {
public:
    // The degree of the Reed-Muller code, which determines the total degree in the underlying multivariate polynomial
    const int NUM_VARIABLES;

    ReedMuller(int num_variables, PolynomialOracle eval_func);

    finite_field::FiniteFieldElement query(const std::vector<finite_field::FiniteFieldElement>& input) const;

    finite_field::FiniteFieldElement operator()(const std::vector<finite_field::FiniteFieldElement>& input)const;

private:
    PolynomialOracle oracle;
};

}

#endif