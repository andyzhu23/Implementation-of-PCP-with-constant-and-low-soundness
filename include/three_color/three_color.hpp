#ifndef THREE_COLOR_HPP
#define THREE_COLOR_HPP

#include <vector>
#include <bitset>

#include "pcp/BitPCP.hpp"

namespace three_color {

using Node = size_t;
using Edge = std::pair<Node, Node>;

enum class Color {
    RED,
    GREEN,
    BLUE
};

std::bitset<2> color_to_bits(Color c);

class ThreeColor {
public:
    ThreeColor(const std::vector<Color> &colors, const std::vector<Edge> &edges);

    ThreeColor(std::vector<Color> &&colors, std::vector<Edge> &&edges);

    const std::vector<Color>& get_colors() const { return colors; }

    const std::vector<std::vector<Node>>& get_adj_list() const { return adj_list; }

    void add_edge(Node u, Node v);

    size_t get_edge_size() const;

    pcp::BitPCP to_BitPCP() const;
private:
    size_t num_edges;
    std::vector<Color> colors;
    std::vector<std::vector<Node>> adj_list;
};

// Generates a random 3-colorable graph
ThreeColor generate_valid_three_coloring_graph(size_t num_nodes, size_t num_edges, size_t num_red, size_t num_green, size_t num_blue);

// Generates a random graph that is not 3-colorable
ThreeColor generate_invalid_three_coloring_graph(size_t num_nodes, size_t num_edges, size_t num_violated_edges, size_t num_red, size_t num_green, size_t num_blue);

}

#endif