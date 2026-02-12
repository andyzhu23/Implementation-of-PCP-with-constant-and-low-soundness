
#ifndef CORE_HPP
#define CORE_HPP

#include "pcp/BitPCP.hpp"
#include "constants.hpp"
#include "pcpp/TesterFactory.hpp"
#include "three_color/ThreeColor.hpp"

namespace core {

// to_expander turns a BitPCP into a BitPCP where the graph is an expander
pcp::BitPCP& to_expander(pcp::BitPCP &pcp, int expanding_coefficient);

// reduce_degree reduces the degree of a BitPCP to degree by replacing each variable with a graph of variables
pcp::BitPCP reduce_degree(const pcp::BitPCP &pcp, int degree);

// gap_amplification amplifies the gap of a BitPCP to constant
pcp::BitPCP gap_amplification(pcp::BitPCP pcp, pcpp::TesterType tester_type);

// Converts a ThreeColor instance to a BitPCP instance
pcp::BitPCP three_color_gap_amplification(const three_color::ThreeColor &tc, pcpp::TesterType tester_type, const std::function<int(size_t)>& iterations_func = constants::DEFAULT_ITERATION_FUNC);
}

#endif
