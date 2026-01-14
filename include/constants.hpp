#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <random>
#include <functional>
#include <cmath>

#include "Aliases.hpp"

namespace constants {

const int CONSTRAINT_COMBINATION_REPETITION = 12;
const int LINEARITY_TEST_REPETITION = 2;
const int CONSISTENCY_ENFORCEMENT_REPETITION = 5;
const int POWERING_RADIUS = 5;
const int EXPANDING_COEFFICIENT = 1;
const int DEGREE = 4;
const unsigned int SAFE_THREAD_NUMBER = 4;
const pcp::Variable PCPVARIABLE_ONE = 1;

const std::function<int(size_t)> DEFAULT_ITERATION_FUNC = [](size_t edge_size) {
    return static_cast<int>(std::ceil(std::log10(edge_size)));
};

// Fixed random seed for reproducibility
inline thread_local std::mt19937 RANDOM_SEED{453};

}

#endif