/*
 * Author: Andy Zhu
 * @date    2025-10-16 14:12:59
 * @version 1.0.0
 */

#ifndef CORE_HPP
#define CORE_HPP

#include "pcp/BitPCP.hpp"
#include "pcp/PoweringPCP.hpp"

namespace core {

// Powering operation on a BitPCP to produce a PoweringPCP from Dinur's PCP proof
pcp::PoweringPCP powering_operation(const pcp::BitPCP &pcp, int radius);

// to_expander turns a BitPCP into a BitPCP where the graph is an expander
pcp::BitPCP& to_expander(pcp::BitPCP &pcp, int expanding_coefficient);

// reduce_degree reduces the degree of a BitPCP to degree by replacing each variable with a graph of variables
pcp::BitPCP reduce_degree(const pcp::BitPCP &pcp, int degree);

// reduce_alphabet reduces the alphabet size of a PoweringPCP to binary using Hadamard codes
pcp::BitPCP reduce_alphabet(const pcp::PoweringPCP &pcp);

}

#endif
