#include <algorithm>
#include <functional>
#include <vector>
#include <random>

#include <iostream>

#include "constants.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "constraint/BitConstraint.hpp"

// Simulated annealing parameters
// starting temperature
const double startingT = 1.0;
// minimum temperature
const double Tmin = 1e-4;
// cooling rate
const double alpha = 0.795;
// iterations per temperature
const size_t iter_per_temp = 100;

std::vector<pcp::BitDomain> get_possible_values(three_csp::Constraint domain_type) {
    std::vector<pcp::BitDomain> possible_values;
    switch (domain_type) {
        case three_csp::Constraint::ENCODED_BINARY:
            possible_values = {
                pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY),
                pcp::BitDomain(1, 1, 1, three_csp::Constraint::ENCODED_BINARY)
            };
            break;
        case three_csp::Constraint::PRODUCT:
            possible_values = {
                pcp::BitDomain(0, 0, 0, three_csp::Constraint::PRODUCT),
                pcp::BitDomain(0, 1, 0, three_csp::Constraint::PRODUCT),
                pcp::BitDomain(1, 0, 0, three_csp::Constraint::PRODUCT),
                pcp::BitDomain(1, 1, 1, three_csp::Constraint::PRODUCT)
            };
            break;
        case three_csp::Constraint::SUM:
            possible_values = {
                pcp::BitDomain(0, 0, 0, three_csp::Constraint::SUM),
                pcp::BitDomain(0, 1, 1, three_csp::Constraint::SUM),
                pcp::BitDomain(1, 0, 1, three_csp::Constraint::SUM),
                pcp::BitDomain(1, 1, 0, three_csp::Constraint::SUM)
            };
            break;
        case three_csp::Constraint::ANY:
            possible_values = {
                pcp::BitDomain(0, 0, 0, three_csp::Constraint::ANY),
                pcp::BitDomain(0, 0, 1, three_csp::Constraint::ANY),
                pcp::BitDomain(0, 1, 0, three_csp::Constraint::ANY),
                pcp::BitDomain(0, 1, 1, three_csp::Constraint::ANY),
                pcp::BitDomain(1, 0, 0, three_csp::Constraint::ANY),
                pcp::BitDomain(1, 0, 1, three_csp::Constraint::ANY),
                pcp::BitDomain(1, 1, 0, three_csp::Constraint::ANY),
                pcp::BitDomain(1, 1, 1, three_csp::Constraint::ANY)
            };
            break;
    }
    return possible_values;
}

namespace analyzer {

double approximate_soundness(pcp::BitPCP pcp) { 
    std::cout << pcp.get_size() << std::endl;
    // Gather constraints list
    const auto &constraints_list = pcp.get_constraints_list();
    size_t m = constraints_list.size();

    if (m == 0) return 1.0; // no constraints

    auto count_satisfied = [&]() {
        int count = 0;
        for (const auto &t : constraints_list) {
            auto [u, v, c] = t;
            if (constraint::evaluateBitConstraint(c, pcp.get_variable(u), pcp.get_variable(v))) {
                ++count;
            }
        }
        return count;
    };

    // Initialize each variable randomly from its domain's possible values
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        auto domain_type = pcp.get_variable(i).get_domain_type();
        auto opts = get_possible_values(domain_type);
        if (!opts.empty()) {
            std::uniform_int_distribution<size_t> dist(0, opts.size() - 1);
            pcp.set_variable(i, opts[dist(constants::RANDOM_SEED)]);
        }
    }

    int current_satisfied = count_satisfied();
    int best_satisfied = current_satisfied;
    std::vector<pcp::BitDomain> best_assignment(pcp.get_size());
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        best_assignment[i] = pcp.get_variable(i);
    }

    std::uniform_int_distribution<pcp::Variable> var_dist(0, static_cast<pcp::Variable>(std::max<size_t>(1, pcp.get_size()) - 1));

    double T = startingT;

    while (T > Tmin) {
        std::cout << "Temperature: " << T << ", Current satisfied: " << current_satisfied << ", Best satisfied: " << best_satisfied << std::endl;
        for (size_t it = 0; it < iter_per_temp; ++it) {
            // pick random variable
            pcp::Variable v = var_dist(constants::RANDOM_SEED);
            auto domain_type = pcp.get_variable(v).get_domain_type();
            auto opts = get_possible_values(domain_type);
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

            // apply candidate
            pcp.set_variable(v, cand);
            int new_satisfied = count_satisfied();
            int delta = new_satisfied - current_satisfied;

            if (delta >= 0) {
                current_satisfied = new_satisfied;
                if (new_satisfied > best_satisfied) {
                    best_satisfied = new_satisfied;
                    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
                        best_assignment[i] = pcp.get_variable(i);
                    }
                }
            } else {
                // accept with probability exp(delta / T) where delta is negative
                double prob = std::exp(static_cast<double>(delta) / T);
                std::uniform_real_distribution<double> ud(0.0, 1.0);
                if (ud(constants::RANDOM_SEED) < prob) {
                    current_satisfied = new_satisfied;
                } else {
                    // revert
                    pcp.set_variable(v, old);
                }
            }
        }
        T *= alpha;
    }

    // restore best assignment
    for (pcp::Variable i = 0; i < static_cast<pcp::Variable>(pcp.get_size()); ++i) {
        pcp.set_variable(i, best_assignment[i]);
    }

    return static_cast<double>(best_satisfied) / static_cast<double>(m);
    
}

}