#include "constants.hpp"
#include "three_color/ThreeColor.hpp"
#include "pcpp/Tester.hpp"

#include "constraint/BitConstraint.hpp"
#include <iostream>

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
            pcpp::Tester tester(colors[u], colors[v]);
            pcp::BitPCP tmp = tester.buildBitPCP();
            // reduce unnecessary variables
            tmp.clean();
            edge_pcps.push_back(std::move(tmp));
            occuring_locations[u].emplace_back(variable_count, 0);
            occuring_locations[v].emplace_back(variable_count, 1);
            variable_count += edge_pcps.back().get_size();
        }
    }

    auto result = pcp::merge_BitPCPs(edge_pcps);

    if (constants::ENFORCING_CONSISTENCY) {

        for (Node u = 0; u < colors.size(); ++u) {
            const auto &locations = occuring_locations[u];
            for (size_t i = 1; i < locations.size(); ++i) {
                size_t x = locations[i].first;
                size_t y = locations[i - 1].first;
                switch (locations[i].second) {
                    case 0:  // position 0: u's bits at (var 0, 1st bit) and (var 0, 3rd bit)
                        switch (locations[i - 1].second) {
                            case 0:  // both at position 0
                                // first bit: (x+0).bit1 == (y+0).bit1
                                result.add_constraint(x, y, constraint::BitConstraint::FIRST_BIT_EQUAL);
                                // second bit: (x+0).bit3 == (y+0).bit3
                                result.add_constraint(x, y, constraint::BitConstraint::THIRD_BIT_EQUAL);
                                break;
                            case 1:  // current at position 0, previous at position 1
                                // first bit: (x+0).bit1 == (y+0).bit2
                                result.add_constraint(x, y, constraint::BitConstraint::FIRST_BIT_EQUAL_SECOND_BIT);
                                // second bit: (x+0).bit3 == (y+1).bit1
                                result.add_constraint(x, y + 1, constraint::BitConstraint::THIRD_BIT_EQUAL_FIRST_BIT);
                                break;
                        }
                        break;
                    case 1:  // position 1: v's bits at (var 0, 2nd bit) and (var 1, 1st bit)
                        switch (locations[i - 1].second) {
                            case 0:  // current at position 1, previous at position 0
                                // first bit: (x+0).bit2 == (y+0).bit1
                                result.add_constraint(x, y, constraint::BitConstraint::SECOND_BIT_EQUAL_FIRST_BIT);
                                // second bit: (x+1).bit1 == (y+0).bit3
                                result.add_constraint(x + 1, y, constraint::BitConstraint::FIRST_BIT_EQUAL_THIRD_BIT);
                                break;
                            case 1:  // both at position 1
                                // first bit: (x+0).bit2 == (y+0).bit2
                                result.add_constraint(x, y, constraint::BitConstraint::SECOND_BIT_EQUAL);
                                // second bit: (x+1).bit1 == (y+1).bit1
                                result.add_constraint(x + 1, y + 1, constraint::BitConstraint::FIRST_BIT_EQUAL);
                                break;
                        }
                        break;
                }
            }
        }
    }

    result.clean();
    return result;
}

}