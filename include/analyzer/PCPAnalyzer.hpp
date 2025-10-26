#ifndef SOUNDNESSVERIFIER_HPP
#define SOUNDNESSVERIFIER_HPP

#include <random>
#include <utility>
#include <vector>

#include "pcp/SimplePCP.hpp"

namespace analyzer {

using Satisfiability = bool;

class PCPAnalyzer {
public:
    PCPAnalyzer(const std::vector<std::pair<pcp::SimplePCP, Satisfiability>> &samples, const int num_trial);

    double getSoundness();

    double getCompleteness();

    double getGap();

private:
    const std::vector<std::pair<pcp::SimplePCP, Satisfiability>> &samples;
    const int num_trial;
    double soundness;
    double completeness;
    double gap;

};

bool query(const pcp::SimplePCP &sample);

}

#endif