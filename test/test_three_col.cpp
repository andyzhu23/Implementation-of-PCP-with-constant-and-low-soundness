// Tests for three_col::Coloring's to_csp and move_to_csp methods
#include <cassert>
#include <iostream>
#include <vector>
#include "three_col.hpp"
#include "csp.hpp"

using namespace three_col;

void test_to_csp() {
	// Simple triangle graph: 0-1-2-0
	std::vector<Color> colors = {Color::RED, Color::GREEN, Color::BLUE};
	std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}};
	Coloring coloring(3, colors, edges);
	auto csp = coloring.to_csp();
	// Check number of nodes
	assert(csp.get_num_node() == 3);
	// Check values
	for (int i = 0; i < 3; ++i) {
		assert(csp.get_value(i) == colors[i]);
	}
	// Check constraints: each edge should have a not-equal constraint
	const auto& constraints = csp.get_constraints();
	int constraint_count = 0;
	for (int i = 0; i < 3; ++i) {
		for (const auto& pr : constraints[i]) {
			int j = pr.first;
			auto& func = pr.second;
            assert(func(colors[i], colors[j])); // Should be not equal
            ++constraint_count;
		}
	}
	// Each edge appears twice (undirected)
	assert(constraint_count == 6);
}

void test_move_to_csp() {
	// Simple line graph: 0-1-2
	std::vector<Color> colors = {Color::RED, Color::GREEN, Color::BLUE};
	std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}};
	Coloring coloring(3, colors, edges);
	auto csp = coloring.move_to_csp();
	// Check number of nodes
	assert(csp.get_num_node() == 3);
	// Check values
	for (int i = 0; i < 3; ++i) {
		assert(csp.get_value(i) == colors[i]);
	}
	// Check constraints: each edge should have a not-equal constraint
	const auto& constraints = csp.get_constraints();
	int constraint_count = 0;
	for (int i = 0; i < 3; ++i) {
		for (const auto& pr : constraints[i]) {
			int j = pr.first;
			auto& func = pr.second;
            assert(func(colors[i], colors[j]));
            ++constraint_count;
		}
	}
	// Each edge appears twice (undirected)
	assert(constraint_count == 4);
}

void test_empty_graph() {
	std::vector<Color> colors = {};
	std::vector<std::pair<int, int>> edges = {};
	Coloring coloring(0, colors, edges);
	auto csp = coloring.to_csp();
	assert(csp.get_num_node() == 0);
	assert(csp.get_constraints().empty());
}

int main() {
	test_to_csp();
	test_move_to_csp();
	test_empty_graph();
	std::cout << "All tests passed!" << std::endl;
	return 0;
}
