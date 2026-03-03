
#ifndef CORE_HPP
#define CORE_HPP

#include "pcp/BinaryCSP.hpp"
#include "constants.hpp"
#include "pcpp/TesterFactory.hpp"
#include "three_color/ThreeColor.hpp"

namespace core {

// to_expander turns a BinaryCSP into a BinaryCSP where the graph is an expander
pcp::BinaryCSP& to_expander(pcp::BinaryCSP &pcp, int expanding_coefficient);

// reduce_degree reduces the degree of a BinaryCSP to degree by replacing each variable with a graph of variables
pcp::BinaryCSP reduce_degree(const pcp::BinaryCSP &pcp, int degree);

// gap_amplification amplifies the gap of a BinaryCSP to constant
pcp::BinaryCSP gap_amplification(pcp::BinaryCSP pcp, pcpp::TesterType tester_type);

// Converts a ThreeColor instance to a BinaryCSP instance
pcp::BinaryCSP three_color_gap_amplification(const three_color::ThreeColor &tc, pcpp::TesterType tester_type, const std::function<int(size_t)>& iterations_func = constants::DEFAULT_ITERATION_FUNC);
}

#endif
