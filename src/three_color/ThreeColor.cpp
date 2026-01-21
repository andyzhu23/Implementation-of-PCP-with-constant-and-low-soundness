#include "three_color/ThreeColor.hpp"
#include "pcpp/ThreeColorTester.hpp"
#include "util/disjoint_set_union.hpp"

void merge_variables(
    std::vector<three_color::Color> colors,
    pcp::BitPCP &pcp, 
    const std::vector<std::vector<std::pair<size_t, int>>> &occuring_location,
    const std::vector<pcp::BitPCP> &reduced_pcps
) {

    util::disjoint_set_union dsu(pcp.get_size());

    for (three_color::Node u = 0; u < colors.size(); ++u) {
        const auto &locations = occuring_location[u];
        for (size_t i = 1; i < locations.size(); ++i) {
            size_t x = locations[i].first;
            size_t y = locations[i - 1].first;
            switch (locations[i].second) {
                case 0:  // position 0: u's bits at (var 0, 1st bit) and (var 0, 3rd bit)
                    switch (locations[i - 1].second) {
                        case 0:  // both at position 0
                            dsu.merge(x, y);
                            dsu.merge(x + 2, y + 2);
                            break;
                        case 1:  // current at position 0, previous at position 1
                            dsu.merge(x, y + 1);
                            dsu.merge(x + 2, y + 3);
                            break;
                    }
                    break;
                case 1:  // position 1: v's bits at (var 0, 2nd bit) and (var 1, 1st bit)
                    switch (locations[i - 1].second) {
                        case 0:  // current at position 1, previous at position 0
                            dsu.merge(x + 1, y);
                            dsu.merge(x + 3, y + 2);
                            break;
                        case 1:  // both at position 1
                            dsu.merge(x + 1, y + 1);
                            dsu.merge(x + 3, y + 3);
                            break;
                    }
                    break;
            }
        }
    }
    
    std::unordered_map<pcp::Variable, pcp::Variable> representative_map;
    size_t new_size = 0;
    for (pcp::Variable i = 0; i < pcp.get_size(); ++i) {
        if (dsu.find(i) == i) {
            representative_map[i] = new_size++;
        }
    }

    pcp::BitPCP new_bitpcp(new_size);

    for (pcp::Variable i = 0; i < pcp.get_size(); ++i) {
        if (dsu.find(i) == i) {
            new_bitpcp.set_variable(
                representative_map[i],
                pcp.get_variable(i)
            );
        }
    }

    for (const auto &[u, v, c] : pcp.get_constraints_list()) {
        pcp::Variable new_u = representative_map[dsu.find(u)];
        pcp::Variable new_v = representative_map[dsu.find(v)];
        new_bitpcp.add_constraint(new_u, new_v, c);
    }
    pcp = std::move(new_bitpcp);
}

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
    size_t variable_count = 0;
    std::vector<std::vector<std::pair<size_t, int>>> occuring_locations(colors.size());
    for (Node u = 0; u < colors.size(); ++u) {
        for (Node v : adj_list[u]) if (v > u) {
            pcpp::ThreeColorTester tester(colors[u], colors[v]);
            pcp::BitPCP tmp = tester.buildBitPCP();
            // reduce unnecessary variables
            edge_pcps.push_back(std::move(tmp));
            occuring_locations[u].emplace_back(variable_count, 0);
            occuring_locations[v].emplace_back(variable_count, 1);
            variable_count += edge_pcps.back().get_size();
        }
    }
    auto result = pcp::merge_BitPCPs(edge_pcps);
    merge_variables(colors, result, occuring_locations, edge_pcps);
    result.clean();
    return result;
}

}