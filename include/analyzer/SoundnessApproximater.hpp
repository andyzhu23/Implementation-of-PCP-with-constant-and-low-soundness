#ifndef SOUNDNESSAPPROXIMATER_HPP
#define SOUNDNESSAPPROXIMATER_HPP

#include "pcp/BinaryCSP.hpp"
#include "constants.hpp"

namespace analyzer {

// Simulated annealing parameters
// starting temperature
const double startingT = 1.0;
// minimum temperature
const double Tmin = 1e-4;
// cooling rate
const double alpha = 0.995;
// iterations per temperature
const size_t iter_per_temp_default = 100000;
// Map from constraint type to possible values in its domain
const std::map<three_csp::Constraint, std::vector<pcp::BinaryDomain>> possible_values = {
    {
        three_csp::Constraint::ENCODED_BINARY,
        {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BinaryDomain(1, 1, 1, three_csp::Constraint::ENCODED_BINARY)
        }
    }, 
    {
        three_csp::Constraint::PRODUCT,
        {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BinaryDomain(0, 1, 0, three_csp::Constraint::PRODUCT),
            pcp::BinaryDomain(1, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BinaryDomain(1, 1, 1, three_csp::Constraint::PRODUCT)
        }
    }, 
    {
        three_csp::Constraint::SUM,
        {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::SUM),
            pcp::BinaryDomain(0, 1, 1, three_csp::Constraint::SUM),
            pcp::BinaryDomain(1, 0, 1, three_csp::Constraint::SUM),
            pcp::BinaryDomain(1, 1, 0, three_csp::Constraint::SUM)
        }
    }, 
    {
        three_csp::Constraint::ANY,
        {
            pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ANY),
            pcp::BinaryDomain(0, 0, 1, three_csp::Constraint::ANY),
            pcp::BinaryDomain(0, 1, 0, three_csp::Constraint::ANY),
            pcp::BinaryDomain(0, 1, 1, three_csp::Constraint::ANY),
            pcp::BinaryDomain(1, 0, 0, three_csp::Constraint::ANY),
            pcp::BinaryDomain(1, 0, 1, three_csp::Constraint::ANY),
            pcp::BinaryDomain(1, 1, 0, three_csp::Constraint::ANY),
            pcp::BinaryDomain(1, 1, 1, three_csp::Constraint::ANY)
        }
    }, 
    {
        three_csp::Constraint::ONE_HOT_COLOR,
        {
            pcp::BinaryDomain(1, 0, 0, three_csp::Constraint::ONE_HOT_COLOR),
            pcp::BinaryDomain(0, 1, 0, three_csp::Constraint::ONE_HOT_COLOR),
            pcp::BinaryDomain(0, 0, 1, three_csp::Constraint::ONE_HOT_COLOR)
        }
    }
};

double approximate_soundness(pcp::BinaryCSP &pcp, size_t iter_per_temp = iter_per_temp_default); 

double approximate_soundness_via_random_subset(pcp::BinaryCSP &pcp);

}

#endif