#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <random>

#include "pcp/Aliases.hpp"

namespace constants {

const int LINEARITY_COEFFICIENT = 10;
const int POWERING_RADIUS = 4;
const int EXPANDING_COEFFICIENT = 1;
const int DEGREE = 4;
const unsigned int SAFE_THREAD_NUMBER = 4;
const pcp::Variable PCPVARIABLE_ONE = 1;

// Fixed random seed for reproducibility
inline thread_local std::mt19937 RANDOM_SEED{453};

}

#endif