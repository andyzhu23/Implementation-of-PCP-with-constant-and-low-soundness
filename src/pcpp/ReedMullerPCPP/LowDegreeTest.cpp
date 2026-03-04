#include "pcpp/ReedMullerPCPP/LowDegreeTest.hpp"

namespace pcpp {

LowDegreeTest::LowDegreeTest(ReedMuller code) : code(code), slope(code.NUM_VARIABLES), intercept(code.NUM_VARIABLES) {
    size_t num_variables = code.NUM_VARIABLES;
    // Generate random slopes and intercepts as random line
    for (size_t i = 0; i < num_variables; ++i) {
        slope[i] = finite_field::get_random_element();
        intercept[i] = finite_field::get_random_element();
    }

    // evaluate the line at roots of unity and use NTT to get univariate polynomial coefficients
    finite_field::FiniteFieldElement omega = finite_field::get_primitive_root_of_unity(num_variables);

}

}