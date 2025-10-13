#ifndef CSP_HPP
#define CSP_HPP

#include <vector>
#include <utility>
#include <functional>

namespace csp {
    template <typename D>
    struct Constraint {
        int v1, v2;
        std::function<bool(D, D)> constraint;
    };

    template <typename D>
    class CSP {
    public:
        CSP(int m_num_node, std::vector<csp::Constraint<D>> cs);
        const std::vector<std::pair<int, std::function<bool(D, D)>>>& get_constraints() const;
        void add_constraint(int v1, int v2, std::function<bool(D, D)> constraint);
        void add_constraint(const csp::Constraint<D> &c);
    private:
        int m_num_node;
        std::vector<std::vector<std::pair<int, std::function<bool(D, D)>>>> m_constraints;
    };
}

#endif