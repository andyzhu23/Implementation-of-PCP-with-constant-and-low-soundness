#include "three_color/three_color.hpp"

namespace three_color {

ThreeColor::ThreeColor(const std::vector<Color> &colors, const std::vector<Edge> &edges) 
    : colors(std::move(colors)), adj_list(this->colors.size()) {
    for (const auto& [u, v] : edges) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }
}

ThreeColor::ThreeColor(std::vector<Color> &&colors, std::vector<Edge> &&edges) 
    : colors(std::move(colors)), adj_list(this->colors.size()) {
    for (const auto& [u, v] : edges) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }
}

void ThreeColor::add_edge(Node u, Node v) {
    adj_list[u].push_back(v);
    adj_list[v].push_back(u);
}

}