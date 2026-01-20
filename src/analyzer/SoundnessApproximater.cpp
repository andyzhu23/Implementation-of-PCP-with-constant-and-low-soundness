#include <algorithm>
#include <functional>
#include <vector>
#include <random>
#include <map>

#include <iostream>

#include "analyzer/SoundnessApproximater.hpp"
#include "constraint/BitConstraint.hpp"

namespace analyzer {

double approximate_soundness(pcp::BitPCP pcp) {
    // Gather constraints list
    const auto &constraints_list = pcp.get_constraints_list();
    size_t m = constraints_list.size();

    if (m == 0) return 1.0; // no constraints

    // Function to count number of satisfied constraints
    auto count_satisfied = [&]() {
        int count = 0;
        for (const auto &[var1, var2, constraint] : constraints_list) {
            pcp::BitDomain val1 = pcp.get_variable(var1);
            pcp::BitDomain val2 = pcp.get_variable(var2);
            count += constraint::evaluateBitConstraint(constraint, val1, val2) ? 1 : 0;
        }
        return count;
    };

    // Function to count number of satisfied constraints involving a specific variable
    auto count_local_satisfied = [&](pcp::Variable changed_var) {
        int count = 0;
        for (const auto &[other_var, constraint] : pcp.get_constraints(changed_var)) {
            pcp::BitDomain val1 = pcp.get_variable(changed_var);
            pcp::BitDomain val2 = pcp.get_variable(other_var);
            count += constraint::evaluateBitConstraint(constraint, val1, val2) ? 1 : 0;
        }
        return count;
    };

    // Initialize each variable randomly from its domain's possible values
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        auto domain_type = pcp.get_variable(i).get_domain_type();
        const auto &opts = possible_values.at(domain_type);
        if (!opts.empty()) {
            std::uniform_int_distribution<size_t> dist(0, opts.size() - 1);
            pcp.set_variable(i, opts[dist(constants::RANDOM_SEED)]);
        }
    }

    int current_satisfied = count_satisfied();
    int best_satisfied = current_satisfied;

    std::uniform_int_distribution<pcp::Variable> var_dist(0, static_cast<pcp::Variable>(std::max<size_t>(1, pcp.get_size()) - 1));

    double T = startingT;

    while (T > Tmin) {
        std::cout << "Temperature: " << T << ", Current satisfied: " << current_satisfied << ", Best satisfied: " << best_satisfied << std::endl;
        for (size_t it = 0; it < iter_per_temp; ++it) {
            // pick random variable
            pcp::Variable v = var_dist(constants::RANDOM_SEED);
            auto domain_type = pcp.get_variable(v).get_domain_type();
            const auto &opts = possible_values.at(domain_type);
            if (opts.size() <= 1) continue; // nothing to change

            // pick a new random value different from current
            std::uniform_int_distribution<size_t> opt_dist(0, opts.size() - 1);
            pcp::BitDomain old = pcp.get_variable(v);
            pcp::BitDomain cand;
            // ensure different
            for (int tries = 0; tries < 10; ++tries) {
                cand = opts[opt_dist(constants::RANDOM_SEED)];
                if (!(cand == old)) break;
            }
            if (cand == old) continue;

            int previous_satisfied = count_local_satisfied(v);
            // apply candidate
            pcp.set_variable(v, cand);
            int new_satisfied = count_local_satisfied(v);
            int delta = new_satisfied - previous_satisfied;

            if (delta >= 0) {
                current_satisfied += delta;
                best_satisfied = std::max(best_satisfied, current_satisfied);
            } else {
                // accept with probability exp(delta / T) where delta is negative
                double prob = std::exp(static_cast<double>(delta) / T);
                std::uniform_real_distribution<double> ud(0.0, 1.0);
                if (ud(constants::RANDOM_SEED) < prob) {
                    current_satisfied += delta;
                } else {
                    // revert
                    pcp.set_variable(v, old);
                }
            }
        }
        T *= alpha;
    }

    return static_cast<double>(best_satisfied) / static_cast<double>(m);
    
}

}