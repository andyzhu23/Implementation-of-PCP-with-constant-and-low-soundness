#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <random>
#include <functional>
#include <cmath>
#include <map>
#include <vector>

#include "Aliases.hpp"

namespace constants {

#ifdef ENFORCE_CONSISTENCY
const int CONSTRAINT_COMBINATION_REPETITION = 200;
const int LINEARITY_TEST_REPETITION = 100;
const int POWERING_RADIUS = 5;
const int DEGREE = 5;
#else
const int CONSTRAINT_COMBINATION_REPETITION = 10;
const int LINEARITY_TEST_REPETITION = 1;
const int POWERING_RADIUS = 5;
const int DEGREE = 5;
#endif
const int EXPANDING_COEFFICIENT = 1;
const unsigned int SAFE_THREAD_NUMBER = 4;
const pcp::Variable PCPVARIABLE_ONE = 1;
const int QUERY_SAMPLING_REPETITION = 10;
const int SUBSET_SIZE = 1000;

const std::function<int(size_t)> DEFAULT_ITERATION_FUNC = [](size_t edge_size) {
    return static_cast<int>(std::ceil(std::log10(edge_size)));
};

// Fixed random seed for reproducibility
inline thread_local std::mt19937 RANDOM_SEED{453};

}

#endif