#include <stdexcept>

#include "core/core.hpp"
#include "pcp/BitPCP.hpp"
#include "util.hpp"

namespace core {

pcp::BitPCP reduce_degree(const pcp::BitPCP &pcp, int degree) {
    if (degree < 3) {
        throw std::invalid_argument("degree must be at least 3");
    }
    size_t original_size = pcp.get_size(), new_size = 0;
    std::vector<size_t> sizes, offsets;
    sizes.reserve(original_size);
    offsets.reserve(original_size);
    for (size_t i = 0; i < original_size; ++i) {
        sizes.push_back(pcp.get_constraints(i).size());
        new_size += sizes.back();
        offsets.push_back(new_size - sizes.back());
    }
    pcp::BitPCP reduced_pcp(new_size);
    
    // Set all variable values first
    for (size_t i = 0; i < original_size; ++i) {
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            reduced_pcp.set_variable(curr, pcp.get_variable(i));
        }
    }
    
    // Connect each variable's copies in a cycle
    for (size_t i = 0; i < original_size; ++i) {
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            size_t next = offsets[i] + (j + 1) % sizes[i];
            reduced_pcp.add_constraint(curr, next, constraint::BitConstraint::EQUAL);
        }
    }
    
    // Map original constraints using constraints_list to get correct constraint types
    const auto &constraints_list = pcp.get_constraints_list();
    for (const auto &[u, v, con] : constraints_list) {
        if (con == constraint::BitConstraint::ANY) continue;
        
        // Find positions of this edge in both adjacency lists
        const auto &u_constraints = pcp.get_constraints(u);
        const auto &v_constraints = pcp.get_constraints(v);
        
        size_t u_pos = 0, v_pos = 0;
        for (size_t i = 0; i < u_constraints.size(); ++i) {
            if (u_constraints[i].first == v) {
                u_pos = i;
                break;
            }
        }
        for (size_t i = 0; i < v_constraints.size(); ++i) {
            if (v_constraints[i].first == u) {
                v_pos = i;
                break;
            }
        }
        
        size_t u_new_index = offsets[u] + u_pos;
        size_t v_new_index = offsets[v] + v_pos;
        reduced_pcp.add_constraint(u_new_index, v_new_index, con);
    }
    
    // Add random edges to reach target degree
    for (size_t i = 0; i < original_size; ++i) {
        util::random_picker<pcp::Variable> rp;
        for (size_t j = 0; j < sizes[i]; ++j) {
            size_t curr = offsets[i] + j;
            rp.add(curr, degree - reduced_pcp.get_constraints(curr).size());
        }
        while (rp.size() > 1) {
            auto [u, v] = rp.pick_two();
            reduced_pcp.add_constraint(u, v, constraint::BitConstraint::ANY);
        }
    }
    
    return reduced_pcp;
}

}