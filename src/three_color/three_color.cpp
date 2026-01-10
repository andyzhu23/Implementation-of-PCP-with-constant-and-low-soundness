#include "three_color/three_color.hpp"
#include "pcpp/Tester.hpp"

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

pcp::BitPCP ThreeColor::to_BitPCP() const {
    std::vector<pcp::BitPCP> edge_pcps;
    for (Node u = 0; u < colors.size(); ++u) {
        for (Node v : adj_list[u]) if (v > u) {
            pcpp::Tester tester(colors[u], colors[v]);
            pcp::BitPCP tmp = tester.buildBitPCP();
            // reduce unnecessary variables
            tmp.clean();
            edge_pcps.push_back(std::move(tmp));
        }
    }
    return pcp::merge_BitPCPs(edge_pcps);
}

}