#ifndef SOUNDNESSAPPROXIMATER_HPP
#define SOUNDNESSAPPROXIMATER_HPP

#include "pcp/BitPCP.hpp"

namespace analyzer {

// Simulated annealing parameters
// starting temperature
const double startingT = 1.0;
// minimum temperature
const double Tmin = 1e-4;
// cooling rate
const double alpha = 0.995;
// iterations per temperature
const size_t iter_per_temp = 10000;

double approximate_soundness(pcp::BitPCP pcp); 

}

#endif