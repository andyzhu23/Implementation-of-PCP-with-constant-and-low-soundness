#include "pcpp/ReedMullerPCPP/LowDegreeTest.hpp"
#include "finite_field/NumberTheoreticTransform.hpp"

namespace pcpp {

LowDegreeTest::LowDegreeTest(ReedMuller code, int degree_bound) : code(code), degree_bound(degree_bound), slope(code.NUM_VARIABLES), intercept(code.NUM_VARIABLES) {
    size_t num_variables = code.NUM_VARIABLES;
    // Generate random slopes and intercepts as random line
    for (size_t i = 0; i < num_variables; ++i) {
        slope[i] = finite_field::get_random_element();
        intercept[i] = finite_field::get_random_element();
    }

    // evaluate the line at roots of unity and use NTT to get univariate polynomial coefficients

    std::vector<finite_field::FiniteFieldElement> evaluations(degree_bound + 1);

    finite_field::padding(evaluations); // Pad to power of 2 for NTT

    finite_field::FiniteFieldElement omega = finite_field::get_primitive_root_of_unity(evaluations.size());
    
    for (size_t i = 0; i < evaluations.size(); ++i) {
        std::vector<finite_field::FiniteFieldElement> point(num_variables);
        finite_field::FiniteFieldElement omega_i = omega.exp(i);
        for (size_t j = 0; j < num_variables; ++j) {
            // point[j] = slope[j] * omega^i + intercept[j]
            point[j] = slope[j] * omega_i + intercept[j];
        }
        evaluations[i] = code.query(point);
    }
    auto coefficients = finite_field::intt(evaluations);
    while (coefficients.size() > degree_bound + 1) {
        coefficients.pop_back(); // Remove higher degree terms beyond degree_bound
    }
    univariate_poly = std::move(coefficients); // Construct univariate polynomial from coefficients
}

const finite_field::UnivariatePolynomial& LowDegreeTest::getUnivariatePolynomial() const {
    return univariate_poly;
}

bool LowDegreeTest::verifyPolynomial() const {
    finite_field::FiniteFieldElement point = finite_field::get_random_element();
    std::vector<finite_field::FiniteFieldElement> query_point(code.NUM_VARIABLES);
    for (size_t j = 0; j < code.NUM_VARIABLES; ++j) {
        query_point[j] = slope[j] * point + intercept[j];
    }
    finite_field::FiniteFieldElement oracle_value = code(query_point);
    finite_field::FiniteFieldElement poly_value = univariate_poly.evaluate(point);
    return oracle_value == poly_value; // Check if the polynomial evaluation matches the oracle
}

}