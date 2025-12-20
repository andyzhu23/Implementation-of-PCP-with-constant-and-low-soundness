#include <chrono>
#include <random>
#include <iostream>

#include "analyzer/PCPAnalyzer.hpp"
#include "constants.hpp"

namespace analyzer {

PCPAnalyzer::PCPAnalyzer(const std::vector<std::pair<pcp::BitPCP, Satisfiability>> &samples, const int num_trial)
 : samples(samples), 
   num_trial(num_trial),
   soundness(0),
   completeness(0) {
    int true_positive = 0;
    int false_positive = 0;
    int accepted = 0;
    for (const auto &[sample, satisfiable] : samples) {
        int cnt = 0;
        for (int i = 0; i < num_trial; ++i) {
            bool satisfied = query(sample);
            cnt += satisfied ? 1 : 0;
        }
        if (satisfiable) {
            ++accepted;
            true_positive += cnt;
        } else {
            false_positive += cnt;
        }
    }

    if (num_trial * accepted == 0) {
        completeness = 1;
    } else {
        completeness = 1.0 * true_positive / num_trial / accepted;
    }

    if (num_trial * (samples.size() - accepted) == 0) {
        soundness = 0;
    } else {
        soundness = 1.0 * false_positive / num_trial / (samples.size() - accepted);
    }
    
    gap = completeness - soundness;
}

double PCPAnalyzer::getSoundness() { return soundness; }

double PCPAnalyzer::getGap() { return gap; };

double PCPAnalyzer::getCompleteness() { return completeness; };

// perform a single uniformly random query on sample
bool query(const pcp::BitPCP &sample) {
    const auto &constraints_list = sample.get_constraints_list();
    if (constraints_list.size() == 0) {
        return true; // no constraints, always satisfied
    }
    std::uniform_int_distribution<int> dist(0, constraints_list.size() - 1);
    int r = dist(constants::RANDOM_SEED);
    const auto &[v1, v2, constraint] = constraints_list[r];
    return constraint::evaluateBitConstraint(constraint, sample.get_variable(v1), sample.get_variable(v2));
}

}