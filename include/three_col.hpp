#ifndef THREE_COL_HPP
#define THREE_COL_HPP

#include <functional>
#include <utility>
#include <vector>

#include "csp.hpp"

namespace three_col {

// Define three colors for 3-coloring problem
enum class Color {
    RED,
    GREEN,
    BLUE,
};
// A simple not-equal constraint for three colors
const std::function<bool(Color, Color)> not_equal = [](Color a, Color b) -> bool { return a != b; };

// A coloring for a graph
class Coloring {
    public:
    // Constructors
    Coloring(int num_node, const std::vector<Color> &colors, const std::vector<std::pair<int, int>> &edges);
    Coloring(int num_node, const std::vector<Color> &colors, std::vector<std::pair<int, int>> &&edges);
    Coloring(int num_node, std::vector<Color> &&colors, const std::vector<std::pair<int, int>> &edges);
    Coloring(int num_node, std::vector<Color> &&colors, std::vector<std::pair<int, int>> &&edges);
    // Getters and setters
    const Color& get_color(int node) const;
    void set_color(int node, Color color);
    const std::vector<std::pair<int, int>>& get_edges() const;

    // Convert the coloring to a 2-CSP instance
    csp::CSP<2, Color> to_csp() const;
    // Move the coloring to a 2-CSP instance
    csp::CSP<2, Color> move_to_csp();
    
    private:
    std::vector<Color> colors;
    std::vector<std::pair<int, int>> edges;
};

}

#endif