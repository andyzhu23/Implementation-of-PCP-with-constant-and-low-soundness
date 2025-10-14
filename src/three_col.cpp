
#include <utility>
#include <vector>
#include <stdexcept>

#include "three_col.hpp"

namespace three_col {

Coloring::Coloring(
    int num_node,
    const std::vector<Color> &colors,
    const std::vector<std::pair<int, int>> &edges
) : colors(colors), edges(edges) {
    if (this->colors.size() != num_node) {
        throw std::invalid_argument("Number of colors must match number of nodes");
    }
}

Coloring::Coloring(
    int num_node,
    const std::vector<Color> &colors,
    std::vector<std::pair<int, int>> &&edges
) : colors(colors), edges(std::move(edges)) {
    if (this->colors.size() != num_node) {
        throw std::invalid_argument("Number of colors must match number of nodes");
    }
}

Coloring::Coloring(
    int num_node,
    std::vector<Color> &&colors,
    const std::vector<std::pair<int, int>> &edges
) : colors(std::move(colors)), edges(edges) {
    if (this->colors.size() != num_node) {
        throw std::invalid_argument("Number of colors must match number of nodes");
    }
}

Coloring::Coloring(
    int num_node,
    std::vector<Color> &&colors,
    std::vector<std::pair<int, int>> &&edges
) : colors(std::move(colors)), edges(std::move(edges)) {
    if (this->colors.size() != num_node) {
        throw std::invalid_argument("Number of colors must match number of nodes");
    }
}

const Color& Coloring::get_color(int node) const {
    return colors[node];
}

void Coloring::set_color(int node, Color color) {
    colors[node] = color;
}

const std::vector<std::pair<int, int>>& Coloring::get_edges() const {
    return edges;
}

// Convert the coloring to a 2-CSP instance

csp::CSP<2, Color> Coloring::to_csp() const {
    std::vector<csp::Constraint<Color>> constraints;
    constraints.reserve(edges.size());
    for (const auto &[u, v] : edges) {
        constraints.emplace_back(u, v, not_equal);
    }
    return csp::CSP<2, Color>(colors.size(), colors, constraints);
}

// Move the coloring to a 2-CSP instance

csp::CSP<2, Color> Coloring::move_to_csp() {
    std::vector<csp::Constraint<Color>> constraints;
    constraints.reserve(edges.size());
    for (const auto &[u, v] : edges) {
        constraints.emplace_back(u, v, not_equal);
    }
    return csp::CSP<2, Color>(colors.size(), std::move(colors), constraints);
}

}