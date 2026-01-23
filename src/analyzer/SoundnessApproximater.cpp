#include <algorithm>
#include <functional>
#include <vector>
#include <random>
#include <map>

#include "constants.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "constraint/BitConstraint.hpp"

namespace analyzer {

double approximate_soundness(pcp::BitPCP &pcp) {
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

double approximate_soundness_via_random_subset(pcp::BitPCP &pcp) {
    auto constraint_list = pcp.get_constraints_list();

    double accumulated_soundness = 0.0;

    for (int _ = 0; _ < constants::QUERY_SAMPLING_REPETITION; ++_) {
        std::shuffle(constraint_list.begin(), constraint_list.end(), constants::RANDOM_SEED);
        pcp::BitPCP sub_pcp;
        std::map<pcp::Variable, pcp::Variable> var_mapping;
        size_t subset_size = std::min<size_t>(constants::SUBSET_SIZE, constraint_list.size());
        for (size_t i = 0; i < subset_size; ++i) {
            auto [old_var1, old_var2, old_constraint] = constraint_list[i];
            if (var_mapping.find(old_var1) == var_mapping.end()) {
                pcp::BitDomain val = pcp.get_variable(old_var1);
                pcp::Variable new_var = static_cast<pcp::Variable>(sub_pcp.get_size());
                sub_pcp.add_variable(val);
                var_mapping[old_var1] = new_var;
            }
            if (var_mapping.find(old_var2) == var_mapping.end()) {
                pcp::BitDomain val = pcp.get_variable(old_var2);
                pcp::Variable new_var = static_cast<pcp::Variable>(sub_pcp.get_size());
                sub_pcp.add_variable(val);
                var_mapping[old_var2] = new_var;
            }
            sub_pcp.add_constraint(
                var_mapping[old_var1],
                var_mapping[old_var2],
                old_constraint
            );
        }

        double soundness_estimate = approximate_soundness(sub_pcp);
        accumulated_soundness += soundness_estimate;
    }
    return accumulated_soundness / static_cast<double>(constants::QUERY_SAMPLING_REPETITION);
}


}