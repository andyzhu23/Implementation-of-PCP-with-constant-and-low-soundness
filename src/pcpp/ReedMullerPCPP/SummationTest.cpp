#include "pcpp/ReedMullerPCPP/SummationTest.hpp"

namespace pcpp {

SummationTest::SummationTest(finite_field::Polynomial p) {
    int num_variables = p.getNumVariables();
    iterations.push_back(std::move(p));
    for (int i = 0; i < num_variables; ++i) {
        random_points.push_back(finite_field::get_random_element());
        iterations.push_back(stripVariable(iterations.back(), random_points.back()));
    }
}

finite_field::Polynomial SummationTest::stripVariable(const finite_field::Polynomial &p, finite_field::FiniteFieldElement point) const {
    std::vector<finite_field::Monomial> new_terms;
    int num_variables = p.getNumVariables() - 1; // We are stripping one variable
    for (const auto &term : p.getTerms()) {
        finite_field::FiniteFieldElement new_coeff = term.getCoefficient();
        std::vector<size_t> new_exps = term.getVariableExps();
        if (new_exps.size() == num_variables + 1) {
            // This term depends on the variable we are stripping
            size_t exp = new_exps.back();
            new_exps.pop_back();
            new_coeff *= point * point.exp(exp); // Substitute the variable with the point value
        }
        new_terms.emplace_back(new_coeff, std::move(new_exps));
    }
    return finite_field::Polynomial(std::move(new_terms));
}

}