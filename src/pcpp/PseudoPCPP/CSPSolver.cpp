#include <functional>
#include <vector>
#include <map>

#include "pcpp/PseudoPCPP/CSPSolver.hpp"
#include "util/disjoint_set_union.hpp"

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
    }, 
    {
        three_csp::Constraint::ONE_HOT_COLOR,
        {
            pcp::BitDomain(1, 0, 0, three_csp::Constraint::ONE_HOT_COLOR),
            pcp::BitDomain(0, 1, 0, three_csp::Constraint::ONE_HOT_COLOR),
            pcp::BitDomain(0, 0, 1, three_csp::Constraint::ONE_HOT_COLOR)
        }
    }
};

// Helper function to merge variables connected by EQUAL constraints and check for contradictions
std::optional<pcp::BitPCP> merge_equal_constraints_helper(const pcp::BitPCP &input) {
    size_t n = input.get_size();
    util::disjoint_set_union dsu(n);
    
    // Merge variables connected by EQUAL constraints
    for (const auto &[var1, var2, constraint] : input.get_constraints_list()) {
        if (constraint == constraint::BitConstraint::EQUAL) {
            dsu.merge(var1, var2);
        }
    }
    
    // Create new BitPCP with merged variables
    std::vector<pcp::BitDomain> new_variables;
    std::map<size_t, size_t> representative_to_index;
    
    for (size_t i = 0; i < n; ++i) {
        size_t rep = dsu.find(i);
        if (representative_to_index.find(rep) == representative_to_index.end()) {
            representative_to_index[rep] = new_variables.size();
            new_variables.push_back(input.get_variable(i));
        }
    }
    
    pcp::BitPCP merged_pcp(std::move(new_variables));
    
    // Add constraints to merged BitPCP
    for (const auto &[var1, var2, constraint] : input.get_constraints_list()) {
        if (constraint != constraint::BitConstraint::EQUAL) {
            size_t rep1 = dsu.find(var1);
            size_t rep2 = dsu.find(var2);
            if (rep1 != rep2) {
                merged_pcp.add_constraint(representative_to_index[rep1], representative_to_index[rep2], constraint);
            } else {
                if (constraint == constraint::BitConstraint::NOTEQUAL) {
                    // Contradiction found, return nullopt to indicate unsatisfiability
                    return std::nullopt;
                }
            }
        }
    }
    
    return merged_pcp;
}

namespace pcpp {

bool check_bitPCP_satisfiability(const pcp::BitPCP &input) {
    // Handle empty BitPCP
    if (input.get_size() == 0) {
        return true;
    }

    std::optional<pcp::BitPCP> merged_pcp_opt = merge_equal_constraints_helper(input);
    if (!merged_pcp_opt.has_value()) {
        return false;
    }
    pcp::BitPCP &bitpcp = merged_pcp_opt.value();

    std::vector<bool> assigned(bitpcp.get_size(), false);
    
    // Recursive backtracking function
    std::function<bool(size_t)> backtrack = [&](size_t var_idx) -> bool {
        // If all variables are assigned, check if solution is complete
        if (var_idx >= bitpcp.get_size()) {
            return true;
        }
        
        assigned[var_idx] = true;
        
        // Try each possible value for current variable
        for (const auto &possible_value : possible_values.at(bitpcp.get_variable(var_idx).get_domain_type())) {
            bitpcp.set_variable(var_idx, possible_value);
            
            // Check consistency with all assigned neighbors
            bool consistent = true;
            for (const auto &[neighbor, constraint] : bitpcp.get_constraints(var_idx)) {
                if (assigned[neighbor]) {
                    if (!constraint::evaluateBitConstraint(constraint, possible_value, bitpcp.get_variable(neighbor))) {
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