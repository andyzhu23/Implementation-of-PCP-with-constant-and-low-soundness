#ifndef CORE_HPP
#define CORE_HPP

#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include "csp.hpp"

namespace core {
    // Get all nodes within a certain radius from a source node in the CSP
    template <typename D>
    std::unordered_map<int, D> get_radius_nodes(const csp::CSP<2, D> &p, int source, int radius) {
        std::queue<std::pair<int, int>> queue;
        queue.emplace(source, 0);
        std::unordered_map<int, D> result;
        const auto &constraints = p.get_constraints();
        while (!queue.empty()) {
            const auto &[node, cnt] = queue.front();
            queue.pop();
            result[node] = p.get_value(node);
            if (cnt < radius) {
                for (auto &[neighbor, _] : constraints[node]) {
                    if (result.find(neighbor) == result.end()) {
                        queue.emplace(neighbor, cnt + 1);
                    }
                }
            }
        }
        return result;
    }

    // Perform the power operation on a 2-CSP with a given radius
    template <typename D>
    csp::CSP<2, std::unordered_map<int, D>> power_operation(const csp::CSP<2, D> &p, int radius) {
        const int num_node = p.get_num_node();
        std::vector<std::unordered_map<int, D>> values;
        values.reserve(num_node);
        for (int i = 0; i < num_node; ++i) {
            values.push_back(get_radius_nodes(p, i, radius));
        }
        csp::CSP<2, std::unordered_map<int, D>> result(num_node, std::move(values));
        for (int i = 0; i < num_node; ++i) {
            for (auto [j, original_constraint] : p.get_constraints()[i]) {
                std::function<bool(const std::unordered_map<int, D>&, const std::unordered_map<int, D>&)> tmp = 
                    [oc = original_constraint, i = i, j = j](
                        const std::unordered_map<int, D> &v1, 
                        const std::unordered_map<int, D> &v2
                    ) -> bool {
                        const std::unordered_map<int, D> *p1 = &v1;
                        const std::unordered_map<int, D> *p2 = &v2;
                        if (p1->size() > p2->size()) {
                            std::swap(p1, p2);
                        }
                        for (auto &[node, value] : *p1) {
                            auto it = p2->find(node);
                            if (it != p2->end()) {
                                if (value != it->second) {
                                    return false;
                                }
                            }
                        }
                        return oc(v1.at(i), v1.at(j));
                    };
                result.add_constraint(i, j, tmp);
            }
        }
        return result;
    }
}

#endif
