/*
 * Author: Andy Zhu
 * @date    2025-11-22 10:27:26
 * @version 1.0.0
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <vector>

#include "constraint/BinaryConstraint.hpp"

namespace pcpp {


// Encode a BinaryConstraint into a vector of bools representing the constraint with linear equation
std::vector<bool> encode_constraint(const constraint::BinaryConstraint& constraint);

}

#endif