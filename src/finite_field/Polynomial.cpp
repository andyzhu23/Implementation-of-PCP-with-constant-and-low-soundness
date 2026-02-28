#include "finite_field/Polynomial.hpp"

namespace finite_field {

Polynomial::Polynomial() : num_variables(0), terms() {}

Polynomial::Polynomial(const std::vector<Monomial> &terms) : num_variables(0), terms(terms) {
    for (const auto &term : terms) {
        num_variables = std::max(num_variables, term.getVariableExps().size());
    }
}

Polynomial::Polynomial(std::vector<Monomial> &&terms) : num_variables(0), terms(std::move(terms)) {
    for (const auto &term : this->terms) {
        num_variables = std::max(num_variables, term.getVariableExps().size());
    }
}

void Polynomial::addTerm(const Monomial &term) {
    terms.push_back(term);
    num_variables = std::max(num_variables, terms.back().getVariableExps().size());
}

void Polynomial::addTerm(Monomial &&term) {
    terms.push_back(std::move(term));
    num_variables = std::max(num_variables, terms.back().getVariableExps().size());
}

FiniteFieldElement Polynomial::evaluate(const std::vector<FiniteFieldElement> &variable_values) const {
    FiniteFieldElement result(0);
    for (const auto &term : terms) {
        result += term.evaluate(variable_values);
    }
    return result;
}

size_t Polynomial::getNumVariables() const {
    return num_variables;
}

const std::vector<Monomial> &Polynomial::getTerms() const {
    return terms;
}

}