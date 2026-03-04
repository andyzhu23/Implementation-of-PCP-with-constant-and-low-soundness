#ifndef FINITE_FIELD_CONSTANT_HPP
#define FINITE_FIELD_CONSTANT_HPP

#include <vector>

#include "finite_field/FiniteFieldElement.hpp"

namespace finite_field {

const size_t FINITE_FIELD_SIZE = 998244353; // Prime used for number theoretic transforms and finite field operations

const FiniteFieldElement FINITE_FIELD_GENERATOR = 3; // A primitive root modulo FINITE_FIELD_SIZE, used for NTT

}

#endif