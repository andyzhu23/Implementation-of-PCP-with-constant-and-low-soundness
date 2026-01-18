#include <algorithm>
#include <functional>
#include <vector>
#include <random>

#include "constants.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "constraint/BitConstraint.hpp"

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
    std::vector<bool> visited(pcp.get_size(), false);

    int satisfied_count = 0;
    int max_satisfied = 0;
    long long iteration_count = constants::SOUNDNESS_APPROXIMATION_ITERATIONS;

    std::function<void(pcp::Variable)> dfs = [&](pcp::Variable u) -> void {
        if (iteration_count-- <= 0) {
            return;
        }
        std::vector<pcp::BitDomain> possible_values = get_possible_values(pcp.get_variable(u).get_domain_type());
        visited[u] = true;

        std::shuffle(possible_values.begin(), possible_values.end(), constants::RANDOM_SEED);

        for (const pcp::BitDomain &val : possible_values) {
            // try setting var to val
            pcp.set_variable(u, val);
            bool satisfied = true;
            // check previous values and constraints
            for (const auto &[v, constraint] : pcp.get_constraints(u)) {
                if (visited[v]) {
                    if (!constraint::evaluateBitConstraint(constraint, pcp.get_variable(u), pcp.get_variable(v))) {
                        satisfied = false;
                        break;
                    }
                }
            }
            if (satisfied) {
                ++satisfied_count;
                max_satisfied = std::max(max_satisfied, satisfied_count);
                // continue DFS
                for (const auto &[v, _] : pcp.get_constraints(u)) {
                    if (!visited[v]) {
                        dfs(v);
                        if (iteration_count <= 0) {
                            return;
                        }
                    }
                }
                --satisfied_count;
            }
        }

    };
    std::uniform_int_distribution<pcp::Variable> distrib(0, static_cast<pcp::Variable>(pcp.get_size() - 1));
    while (iteration_count > 0) {
        pcp::Variable start_var = distrib(constants::RANDOM_SEED);
        dfs(start_var);
    }
    return static_cast<double>(max_satisfied) / pcp.get_size();
}

}