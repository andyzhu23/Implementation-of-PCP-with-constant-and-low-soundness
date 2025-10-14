#ifndef CSP_HPP
#define CSP_HPP

#include <functional>
#include <utility>
#include <vector>

namespace csp {
    // A binary constraint between two variables of type D
    template <typename D>
    struct Constraint {
        int v1, v2;
        std::function<bool(const D&, const D&)> constraint;
    };

    template <int k, typename D>
    class CSP {

    };

    // Specialization for two-CSP (k=2)
    template <typename D>
    class CSP<2, D> {
      public:
        // Constructors
        CSP(int m_num_node, std::vector<D> &values) : m_num_node(m_num_node), values(values), m_constraints(m_num_node) {}
        // Constructor with rvalue reference for values
        CSP(int m_num_node, std::vector<D> &&values) : m_num_node(m_num_node), values(values), m_constraints(m_num_node) {}
        // Constructor with constraints
        CSP(int m_num_node, std::vector<D> &values, std::vector<Constraint<D>> &cs) 
            : m_num_node(m_num_node), values(values) {
            m_constraints.resize(m_num_node);
            for (const auto &c : cs) {
                add_constraint(c);
            }
        }
        // Constructor with rvalue reference for values and lvalue reference constraints
        CSP(int m_num_node, std::vector<D> &&values, std::vector<Constraint<D>> &cs) 
            : m_num_node(m_num_node), values(values) {
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