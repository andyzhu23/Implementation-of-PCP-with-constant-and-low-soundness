#ifndef CSP_HPP
#define CSP_HPP

#include <functional>
#include <utility>
#include <vector>

namespace csp {


// General template for K-CSP (not planned to be implemented for the Part II project)
template <int K, typename D>
class CSP {
    static_assert(K == 2, "Only K=2 is supported 2-query PCP");
};


// A binary constraint between two variables of type D
template <typename D>
struct Constraint {
    int v1, v2;
    std::function<bool(const D&, const D&)> constraint;

	// Constructor
	Constraint(int v1, int v2, std::function<bool(const D&, const D&)> constraint)
		: v1(v1), v2(v2), constraint(constraint) {}
};


// Specialization for two-CSP (K=2)
template <typename D>
class CSP<2, D> {
public:
    // Constructors
    CSP(int m_num_node, const std::vector<D> &values)
        : m_num_node(m_num_node), values(values), m_constraints(m_num_node) {
        if (this->values.size() != m_num_node) {
            throw std::invalid_argument("Number of values must match number of nodes");
        }
    }
    // Constructor with rvalue reference for values
    CSP(int m_num_node, std::vector<D> &&values)
        : m_num_node(m_num_node), values(values), m_constraints(m_num_node) {
        if (this->values.size() != m_num_node) {
            throw std::invalid_argument("Number of values must match number of nodes");
        }
    }
    // Constructor with constraints
    CSP(int m_num_node, const std::vector<D> &values, const std::vector<Constraint<D>> &cs)
        : m_num_node(m_num_node), values(values) {
        if (this->values.size() != m_num_node) {
            throw std::invalid_argument("Number of values must match number of nodes");
        }
        m_constraints.resize(m_num_node);
        for (const auto &c : cs) {
            add_constraint(c);
        }
    }
    // Constructor with rvalue reference for values and lvalue reference constraints
    CSP(int m_num_node, std::vector<D> &&values, const std::vector<Constraint<D>> &cs)
        : m_num_node(m_num_node), values(values) {
        if (this->values.size() != m_num_node) {
            throw std::invalid_argument("Number of values must match number of nodes");
        }
        m_constraints.resize(m_num_node);
        for (const auto &c : cs) {
            add_constraint(c);
        }
    }

    // Accessors
    const std::vector<std::vector<std::pair<int, std::function<bool(const D&, const D&)>>>>& get_constraints() const {
        return m_constraints;
    }
    const D& get_value(int v) const {
        return values[v];
    }
    int get_num_node() const {
        return m_num_node;
    }

    // Mutators
    void add_constraint(int v1, int v2, std::function<bool(const D&, const D&)> constraint) {
        m_constraints[v1].emplace_back(v2, constraint);
        m_constraints[v2].emplace_back(v1, constraint);
    }
    void add_constraint(const csp::Constraint<D> &c) {
        m_constraints[c.v1].emplace_back(c.v2, c.constraint);
        m_constraints[c.v2].emplace_back(c.v1, c.constraint);
    }

private:
    // number of variables
    int m_num_node;
    // adjacency list representation of constraints
    std::vector<std::vector<std::pair<int, std::function<bool(const D&, const D&)>>>> m_constraints;
    // values associated with each variable
    std::vector<D> values;
};

}

#endif