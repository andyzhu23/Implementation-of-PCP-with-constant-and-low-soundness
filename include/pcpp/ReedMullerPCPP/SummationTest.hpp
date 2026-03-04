#ifndef SUMMATION_TEST_HPP
#define SUMMATION_TEST_HPP

#include "pcpp/ReedMullerPCPP/ReedMuller.hpp"

namespace pcpp {

class SummationTest {
public:

    // This class represents the result of stripping a variable from a polynomial in summation test
    struct StripVariableResult {
        // g is the univariate polynomial where the stripped variable is the only variable, and the other variables are summed over
        ReedMuller g;
        // r is the value that the stripped variable is set to in h; h should equal the original polynomial with the stripped variable set to r, and all other variables summed over
        finite_field::FiniteFieldElement r;
        // h is the multivariate polynomial where the stripped variable is set to r, and all other variables are summed over; h should equal the original polynomial with the stripped variable set to r, and all other variables summed over
        ReedMuller h;

        StripVariableResult();
    };
    
    SummationTest(ReedMuller code, int var_range);

    StripVariableResult stripVariable(int variable_index);

private:
    ReedMuller code;
    // var_range is the range of values for each variables summing over
    int var_range;
};

}

#endif