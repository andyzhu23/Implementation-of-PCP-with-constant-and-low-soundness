#include <functional>
#include <vector>
#include <map>
#include <optional>

#include "pcpp/PseudoPCPP/CSPSolver.hpp"
#include "util/disjoint_set_union.hpp"

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

// Helper function to merge variables connected by EQUAL constraints and check for contradictions
std::optional<pcp::BinaryCSP> merge_equal_constraints_helper(const pcp::BinaryCSP &input) {
    size_t n = input.get_size();
    util::disjoint_set_union dsu(n);
    
    // Merge variables connected by EQUAL constraints
    for (const auto &[var1, var2, constraint] : input.get_constraints_list()) {
        if (constraint == constraint::BinaryConstraint::EQUAL) {
            dsu.merge(var1, var2);
        }
    }
    
    // Create new BinaryCSP with merged variables
    std::vector<pcp::BinaryDomain> new_variables;
    std::map<size_t, size_t> representative_to_index;
    
    for (size_t i = 0; i < n; ++i) {
        size_t rep = dsu.find(i);
        if (representative_to_index.find(rep) == representative_to_index.end()) {
            representative_to_index[rep] = new_variables.size();
            new_variables.push_back(input.get_variable(i));
        }
    }
    
    pcp::BinaryCSP merged_pcp(std::move(new_variables));
    
    // Add constraints to merged BinaryCSP
    for (const auto &[var1, var2, constraint] : input.get_constraints_list()) {
        if (constraint != constraint::BinaryConstraint::EQUAL) {
            size_t rep1 = dsu.find(var1);
            size_t rep2 = dsu.find(var2);
            if (rep1 != rep2) {
                merged_pcp.add_constraint(representative_to_index[rep1], representative_to_index[rep2], constraint);
            } else {
                if (constraint == constraint::BinaryConstraint::NOTEQUAL) {
                    // Contradiction found, return nullopt to indicate unsatisfiability
                    return std::nullopt;
                }
            }
        }
    }
    
    return merged_pcp;
}

namespace pcpp {

bool check_BinaryCSP_satisfiability(const pcp::BinaryCSP &input) {
    // Handle empty BinaryCSP
    if (input.get_size() == 0) {
        return true;
    }

    std::optional<pcp::BinaryCSP> merged_pcp_opt = merge_equal_constraints_helper(input);
    if (!merged_pcp_opt.has_value()) {
        return false;
    }
    pcp::BinaryCSP &BinaryCSP = merged_pcp_opt.value();

    std::vector<bool> assigned(BinaryCSP.get_size(), false);
    
    // Recursive backtracking function
    std::function<bool(size_t)> backtrack = [&](size_t var_idx) -> bool {
        // If all variables are assigned, check if solution is complete
        if (var_idx >= BinaryCSP.get_size()) {
            return true;
        }
        
        assigned[var_idx] = true;
        
        // Try each possible value for current variable
        for (const auto &possible_value : possible_values.at(BinaryCSP.get_variable(var_idx).get_domain_type())) {
            BinaryCSP.set_variable(var_idx, possible_value);
            
            // Check consistency with all assigned neighbors
            bool consistent = true;
            for (const auto &[neighbor, constraint] : BinaryCSP.get_constraints(var_idx)) {
                if (assigned[neighbor]) {
                    if (!constraint::evaluateBinaryConstraint(constraint, possible_value, BinaryCSP.get_variable(neighbor))) {
                        consistent = false;
                        break;
                    }
                }
            }
            
            // If consistent, try next variable
            if (consistent && backtrack(var_idx + 1)) {
                return true;
            }
        }
        
        assigned[var_idx] = false;
        return false;
    };
    
    return backtrack(0);
}

}