#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <random>
#include <functional>
#include <cmath>
#include <map>
#include <vector>

#include "Aliases.hpp"

namespace constants {

const int CONSTRAINT_COMBINATION_REPETITION = 20;
#ifdef ENFORCE_CONSISTENCY
const int LINEARITY_TEST_REPETITION = 10;
#else
const int LINEARITY_TEST_REPETITION = 0;
#endif
const int POWERING_RADIUS = 5;
const int EXPANDING_COEFFICIENT = 1;
const int DEGREE = 5;
const unsigned int SAFE_THREAD_NUMBER = 4;
const pcp::Variable PCPVARIABLE_ONE = 1;

const std::function<int(size_t)> DEFAULT_ITERATION_FUNC = [](size_t edge_size) {
    return static_cast<int>(std::ceil(std::log10(edge_size)));
};

// Fixed random seed for reproducibility
inline thread_local std::mt19937 RANDOM_SEED{453};

}

#endif