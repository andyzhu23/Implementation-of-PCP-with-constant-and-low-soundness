#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <random>
#include <functional>
#include <cmath>
#include <map>
#include <vector>

#include "Aliases.hpp"

namespace constants {

const int CONSTRAINT_COMBINATION_REPETITION = 2000;
#ifdef ENFORCE_CONSISTENCY
const int LINEARITY_TEST_REPETITION = 1000;
#else
const int LINEARITY_TEST_REPETITION = 1;
#endif
const int POWERING_RADIUS = 10;
const int EXPANDING_COEFFICIENT = 1;
const int DEGREE = 10;
const unsigned int SAFE_THREAD_NUMBER = 4;
const pcp::Variable PCPVARIABLE_ONE = 1;

const std::function<int(size_t)> DEFAULT_ITERATION_FUNC = [](size_t edge_size) {
    return static_cast<int>(std::ceil(std::log10(edge_size)));
};

// Fixed random seed for reproducibility
inline thread_local std::mt19937 RANDOM_SEED{453};

// Map from constraint type to possible values in its domain
const std::map<three_csp::Constraint, std::vector<pcp::BitDomain>> possible_values = {
    {
        three_csp::Constraint::ENCODED_BINARY,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BitDomain(1, 1, 1, three_csp::Constraint::ENCODED_BINARY)
        }
    }, 
    {
        three_csp::Constraint::PRODUCT,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BitDomain(0, 1, 0, three_csp::Constraint::PRODUCT),
            pcp::BitDomain(1, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BitDomain(1, 1, 1, three_csp::Constraint::PRODUCT)
        }
    }, 
    {
        three_csp::Constraint::SUM,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::SUM),
            pcp::BitDomain(0, 1, 1, three_csp::Constraint::SUM),
            pcp::BitDomain(1, 0, 1, three_csp::Constraint::SUM),
            pcp::BitDomain(1, 1, 0, three_csp::Constraint::SUM)
        }
    }, 
    {
        three_csp::Constraint::ANY,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(0, 0, 1, three_csp::Constraint::ANY),
            pcp::BitDomain(0, 1, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(0, 1, 1, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 0, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 0, 1, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 1, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 1, 1, three_csp::Constraint::ANY)
        }
    }
};

}

#endif