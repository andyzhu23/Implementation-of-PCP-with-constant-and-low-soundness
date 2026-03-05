#include "pcpp/ReedMullerPCPP/ReedMuller.hpp"

namespace pcpp {

ReedMuller::ReedMuller(int num_variables, PolynomialOracle eval_func) : NUM_VARIABLES(num_variables), oracle(std::move(eval_func)) {}

finite_field::FiniteFieldElement ReedMuller::query(const std::vector<finite_field::FiniteFieldElement>& input) const {
    return oracle(input);
}

finite_field::FiniteFieldElement ReedMuller::operator()(const std::vector<finite_field::FiniteFieldElement>& input) const {
    return query(input);
}

}