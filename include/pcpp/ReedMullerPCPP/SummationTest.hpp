#ifndef SUMMATION_TEST_HPP
#define SUMMATION_TEST_HPP

#include "finite_field/Polynomial.hpp"

namespace pcpp {

class SummationTest {
public:
    // Initializes the summation test with the given polynomial and creates iterations by stripping variables
    SummationTest(finite_field::Polynomial p);

    // Strips the last variable from the polynomial by substituting it with a random point
    finite_field::Polynomial stripVariable(const finite_field::Polynomial &p, finite_field::FiniteFieldElement point) const;

private:
    std::vector<finite_field::Polynomial> iterations;
    std::vector<finite_field::FiniteFieldElement> random_points;
};

}

#endif