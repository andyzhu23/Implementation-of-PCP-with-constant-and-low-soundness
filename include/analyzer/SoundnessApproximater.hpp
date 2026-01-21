#ifndef SOUNDNESSAPPROXIMATER_HPP
#define SOUNDNESSAPPROXIMATER_HPP

#include "pcp/BitPCP.hpp"
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
const size_t iter_per_temp = 100000;
// Map from constraint type to possible values in its domain
const std::map<three_csp::Constraint, std::vector<pcp::BitDomain>> possible_values = {
    {
        three_csp::Constraint::ENCODED_BINARY,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
            pcp::BitDomain(1, 1, 1, three_csp::Constraint::ENCODED_BINARY)
        }
    }, 
    {
        three_csp::Constraint::PRODUCT,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BitDomain(0, 1, 0, three_csp::Constraint::PRODUCT),
            pcp::BitDomain(1, 0, 0, three_csp::Constraint::PRODUCT),
            pcp::BitDomain(1, 1, 1, three_csp::Constraint::PRODUCT)
        }
    }, 
    {
        three_csp::Constraint::SUM,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::SUM),
            pcp::BitDomain(0, 1, 1, three_csp::Constraint::SUM),
            pcp::BitDomain(1, 0, 1, three_csp::Constraint::SUM),
            pcp::BitDomain(1, 1, 0, three_csp::Constraint::SUM)
        }
    }, 
    {
        three_csp::Constraint::ANY,
        {
            pcp::BitDomain(0, 0, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(0, 0, 1, three_csp::Constraint::ANY),
            pcp::BitDomain(0, 1, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(0, 1, 1, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 0, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 0, 1, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 1, 0, three_csp::Constraint::ANY),
            pcp::BitDomain(1, 1, 1, three_csp::Constraint::ANY)
        }
    }
};

double approximate_soundness(pcp::BitPCP pcp); 

}

#endif