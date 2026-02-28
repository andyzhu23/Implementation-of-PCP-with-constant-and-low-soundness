#include "pcpp/ReedMullerPCPP/LowDegreeTest.hpp"

namespace pcpp {

LowDegreeTest::LowDegreeTest(finite_field::Polynomial p) : slope(p.getNumVariables()), intercept(p.getNumVariables()) {
    size_t num_variables = p.getNumVariables();
    // Generate random slopes and intercepts as random line
    for (size_t i = 0; i < num_variables; ++i) {
        slope[i] = finite_field::get_random_element();
        intercept[i] = finite_field::get_random_element();
    }

    std::map<size_t, finite_field::FiniteFieldElement> coefficients;

    for (const finite_field::Monomial &term : p.getTerms()) {
        size_t total = 0;
        for (size_t exp : term.getVariableExps()) {
            total += exp;
        }
        coefficients[total] += term.evaluate(slope);
        coefficients[0] += term.evaluate(intercept);
    }

    h = finite_field::UnivariatePolynomial(std::move(coefficients));
}

}