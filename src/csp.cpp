#include "csp.hpp"

template <typename D>
csp::CSP<D>::CSP(int m_num_node, std::vector<csp::Constraint<D>> cs) : m_num_node(m_num_node) {
    m_constraints.resize(m_num_node);
    for (const auto &c : cs) {
        add_constraint(c);
    }
}

template <typename D>
const std::vector<std::pair<int, std::function<bool(D, D)>>>& csp::CSP<D>::get_constraints() const {
    return m_constraints;
}

template <typename D>
void csp::CSP<D>::add_constraint(int v1, int v2, std::function<bool(D, D)> constraint) {
    m_constraints[v1].emplace_back(v2, constraint);
    m_constraints[v2].emplace_back(v1, constraint);
}

template <typename D>
void csp::CSP<D>::add_constraint(const csp::Constraint<D> &c) {
    m_constraints[c.v1].emplace_back(c.v2, c.constraint);
    m_constraints[c.v2].emplace_back(c.v1, c.constraint);
}