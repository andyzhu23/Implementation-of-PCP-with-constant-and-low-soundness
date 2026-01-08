#include <chrono>
#include <random>
#include <stdexcept>

#include "constants.hpp"
#include "three_color/three_color.hpp"

namespace three_color {

ThreeColor generate_valid_three_coloring_graph(size_t num_nodes, size_t num_edges, size_t num_red, size_t num_green, size_t num_blue) {
    if (num_red + num_green + num_blue != num_nodes) {
        throw std::invalid_argument("Sum of color counts must equal number of nodes");
    }
    std::vector<Color> colors(num_nodes);
    for (size_t i = 0; i < num_red; ++i) {
        colors[i] = Color::RED;
    }
    for (size_t i = num_red; i < num_red + num_green; ++i) {
        colors[i] = Color::GREEN;
    }
    for (size_t i = num_red + num_green; i < num_nodes; ++i) {
        colors[i] = Color::BLUE;
    }

    // shuffle colors to randomize node colors
    std::shuffle(colors.begin(), colors.end(), constants::RANDOM_SEED);
    std::vector<Node> red_nodes;
    std::vector<Node> green_nodes;
    std::vector<Node> blue_nodes;
    for (size_t i = 0; i < num_nodes; ++i) {
        switch (colors[i]) {
            case Color::RED:
                red_nodes.push_back(i);
                break;
            case Color::GREEN:
                green_nodes.push_back(i);
                break;
            case Color::BLUE:
                blue_nodes.push_back(i);
                break;
        }
    }

    std::uniform_int_distribution<size_t> red_dist(0, red_nodes.size() - 1);
    std::uniform_int_distribution<size_t> green_dist(0, green_nodes.size() - 1);
    std::uniform_int_distribution<size_t> blue_dist(0, blue_nodes.size() - 1);

    // Generate edges only between different color nodes
    std::vector<Edge> edges;

    for (size_t i = 0; i < num_edges; ++i) {
        int choice = constants::RANDOM_SEED() % 3;
        Node u, v;
        switch (choice) {
            case 0: // RED-GREEN
                u = red_nodes[red_dist(constants::RANDOM_SEED)];
                v = green_nodes[green_dist(constants::RANDOM_SEED)];
                break;
            case 1: // GREEN-BLUE
                u = green_nodes[green_dist(constants::RANDOM_SEED)];
                v = blue_nodes[blue_dist(constants::RANDOM_SEED)];
                break;
            case 2: // BLUE-RED
                u = blue_nodes[blue_dist(constants::RANDOM_SEED)];
                v = red_nodes[red_dist(constants::RANDOM_SEED)];
                break;
        }
        edges.emplace_back(u, v);
    }
    return ThreeColor(std::move(colors), std::move(edges));
}

ThreeColor generate_invalid_three_coloring_graph(size_t num_nodes, size_t num_edges, size_t num_violated_edges, size_t num_red, size_t num_green, size_t num_blue) {
    if (num_violated_edges > num_edges) {
        throw std::invalid_argument("Number of violated edges cannot exceed total number of edges");
    }
    ThreeColor graph = generate_valid_three_coloring_graph(num_nodes, num_edges - num_violated_edges, num_red, num_green, num_blue);

    std::vector<Node> red_nodes;
    std::vector<Node> green_nodes;
    std::vector<Node> blue_nodes;
    for (size_t i = 0; i < num_nodes; ++i) {
        switch (graph.get_colors()[i]) {
            case Color::RED:
                red_nodes.push_back(i);
                break;
            case Color::GREEN:
                green_nodes.push_back(i);
                break;
            case Color::BLUE:
                blue_nodes.push_back(i);
                break;
        }
    }

    // Add violated edges between same color nodes

    std::uniform_int_distribution<size_t> red_dist(0, red_nodes.size() - 1);
    std::uniform_int_distribution<size_t> green_dist(0, green_nodes.size() - 1);
    std::uniform_int_distribution<size_t> blue_dist(0, blue_nodes.size() - 1);


    for (size_t i = 0; i < num_violated_edges; ++i) {
        int choice = constants::RANDOM_SEED() % 3;
        Node u, v;
        switch (choice) {
            case 0: // RED-RED
                u = red_nodes[red_dist(constants::RANDOM_SEED)];
                v = red_nodes[red_dist(constants::RANDOM_SEED)];
                break;
            case 1: // GREEN-GREEN
                u = green_nodes[green_dist(constants::RANDOM_SEED)];
                v = green_nodes[green_dist(constants::RANDOM_SEED)];
                break;
            case 2: // BLUE-BLUE
                u = blue_nodes[blue_dist(constants::RANDOM_SEED)];
                v = blue_nodes[blue_dist(constants::RANDOM_SEED)];
                break;
        }
        graph.add_edge(u, v);
    }

    return graph;
}

}