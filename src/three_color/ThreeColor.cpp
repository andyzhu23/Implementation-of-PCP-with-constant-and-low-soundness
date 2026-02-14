#include "three_color/ThreeColor.hpp"
#include "util/disjoint_set_union.hpp"
#include "pcpp/TesterFactory.hpp"

namespace three_color {

std::bitset<2> color_to_bits(Color c) {
    std::bitset<2> result;
    switch (c) {
        case Color::RED:
            result[0] = 0;
            result[1] = 0;
            break;
        case Color::GREEN:
            result[0] = 0;
            result[1] = 1;
            break;
        case Color::BLUE:
            result[0] = 1;
            result[1] = 0;
            break;
    }
    return result;
}

ThreeColor::ThreeColor(const std::vector<Color> &colors, const std::vector<Edge> &edges) 
    : num_edges(edges.size()), colors(std::move(colors)), adj_list(this->colors.size()) {
    for (const auto &[u, v] : edges) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }
}

ThreeColor::ThreeColor(std::vector<Color> &&colors, std::vector<Edge> &&edges) 
    : num_edges(edges.size()), colors(std::move(colors)), adj_list(this->colors.size()) {
    for (const auto& [u, v] : edges) {
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
    }
}

void ThreeColor::add_edge(Node u, Node v) {
    adj_list[u].push_back(v);
    adj_list[v].push_back(u);
}

size_t ThreeColor::get_edge_size() const { return num_edges; }

pcp::BitPCP ThreeColor::to_BitPCP(pcpp::TesterType tester_type) const {
    std::unique_ptr<pcpp::Tester> tester = pcpp::get_tester(tester_type);
    return tester->three_color_to_bitpcp(*this);
}

}