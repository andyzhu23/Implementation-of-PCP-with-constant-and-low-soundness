#include <iostream>
#include <vector>
#include <functional>

#include "three_color/ThreeColor.hpp"

#include <cassert>
#include <stdexcept>

std::vector<std::function<void()>> test_cases = {
    // Test 1: Generated three-colorable graph has correct color counts
    []() -> void {
        size_t N = 12;
        size_t E = 20;
        size_t num_red = 3;
        size_t num_green = 4;
        size_t num_blue = 5;
        auto graph = three_color::generate_valid_three_coloring_graph(N, E, num_red, num_green, num_blue);
        auto colors = graph.get_colors();
        assert(colors.size() == N);
        size_t r = 0, g = 0, b = 0;
        for (auto c : colors) {
            switch (c) {
                case three_color::Color::RED: ++r; break;
                case three_color::Color::GREEN: ++g; break;
                case three_color::Color::BLUE: ++b; break;
            }
        }
        assert(r == num_red);
        assert(g == num_green);
        assert(b == num_blue);
    },

    // Test 1b: Generated three-colorable graph has no monochromatic edges
    []() -> void {
        size_t N = 12;
        size_t E = 30;
        size_t num_red = 4;
        size_t num_green = 4;
        size_t num_blue = 4;
        auto graph = three_color::generate_valid_three_coloring_graph(N, E, num_red, num_green, num_blue);
        auto colors = graph.get_colors();
        auto const &adj = graph.get_adj_list();
        assert(adj.size() == N);
        for (size_t u = 0; u < adj.size(); ++u) {
            for (auto v : adj[u]) {
                // check undirected edge once
                if (v > u) {
                    assert(colors[u] != colors[v]);
                }
            }
        }
    },

    // Test 2: generate_non_three_colorable_graph throws when violated > edges
    []() -> void {
        bool thrown = false;
        try {
            // request more violated edges than total edges -> should throw
            three_color::generate_invalid_three_coloring_graph(5, 2, 3, 1, 2, 2);
        } catch (const std::invalid_argument &) {
            thrown = true;
        }
        assert(thrown);
    },

    // Test 2b: generate_non_three_colorable_graph produces at least the requested number of violated edges
    []() -> void {
        size_t N = 50;
        size_t E = 80;
        size_t violated = 7;
        size_t num_red = 17, num_green = 16, num_blue = 17;
        auto graph = three_color::generate_invalid_three_coloring_graph(N, E, violated, num_red, num_green, num_blue);
        auto colors = graph.get_colors();
        auto const &adj = graph.get_adj_list();
        // Count unique violated edges (u < v) where colors equal
        size_t count = 0;
        for (size_t u = 0; u < adj.size(); ++u) {
            for (auto v : adj[u]) {
                if (colors[u] == colors[v]) ++count;
            }
        }
        assert(count == violated * 2);
    },

    // Test 3: Repeated generation produces different shuffles (randomness sanity)
    []() -> void {
        size_t N = 30;
        size_t E = 50;
        size_t num_red = 10, num_green = 10, num_blue = 10;
        bool different_found = false;
        for (int i = 0; i < 5 && !different_found; ++i) {
            auto a = three_color::generate_valid_three_coloring_graph(N, E, num_red, num_green, num_blue);
            auto b = three_color::generate_valid_three_coloring_graph(N, E, num_red, num_green, num_blue);
            if (a.get_colors() != b.get_colors()) different_found = true;
        }
        // It's possible (but extremely unlikely) that all shuffles match; require at least one difference
        assert(different_found);
    },

    // Test 4: ThreeColor constructors preserve provided colors (lvalue and rvalue overloads)
    []() -> void {
        using Edge = three_color::Edge;
        std::vector<three_color::Color> colors = {
            three_color::Color::RED,
            three_color::Color::GREEN,
            three_color::Color::BLUE,
            three_color::Color::RED
        };
        std::vector<Edge> edges = { {0,1}, {1,2}, {2,3} };
        three_color::ThreeColor tc1(colors, edges);
        assert(tc1.get_colors() == colors);

        auto colors2 = colors;
        auto edges2 = edges;
        three_color::ThreeColor tc2(std::move(colors2), std::move(edges2));
        // tc2 should have the same number of colors as original
        assert(tc2.get_colors().size() == colors.size());
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        // Run each test case multiple times to catch randomness issues
        for (int trial = 0; trial < 3; ++trial) {
            test_cases[i]();
        }
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
