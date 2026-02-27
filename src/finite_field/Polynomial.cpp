#include "finite_field/Polynomial.hpp"

namespace finite_field {

Polynomial::Polynomial() : terms() {}

Polynomial::Polynomial(const std::vector<Monomial> &terms) : terms(terms) {}

Polynomial::Polynomial(std::vector<Monomial> &&terms) : terms(std::move(terms)) {}

void Polynomial::addTerm(const Monomial &term) {
    terms.push_back(term);
}

void Polynomial::addTerm(Monomial &&term) {
    terms.push_back(std::move(term));
}

FiniteFieldElement Polynomial::evaluate(const std::vector<FiniteFieldElement> &variable_values) const {
    FiniteFieldElement result(0);
    for (const auto &term : terms) {
        result += term.evaluate(variable_values);
    }
    return result;
}

}