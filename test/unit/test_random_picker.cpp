// Unit tests for util::random_picker
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "util.hpp"

std::vector<std::function<void()>> test_cases = {
	// Test 1: basic add/pick behavior with two items (counts = 1 each)
	[]() -> void {
		util::random_picker<int> rp;
		rp.add(1, 1);
		rp.add(2, 1);
		assert(rp.size() == 2);
		auto p = rp.pick_two();
		// Should return two distinct items and both must be among {1,2}
		assert((p.first == 1 || p.first == 2));
		assert((p.second == 1 || p.second == 2));
		assert(p.first != p.second);
		// Both had count 1, so they should be removed after pick_two
		assert(rp.size() == 0);
	},

	// Test 2: one item with count >1 and another with count 1
	[]() -> void {
		util::random_picker<int> rp;
		rp.add(10, 2);
		rp.add(20, 1);
		assert(rp.size() == 2);
		auto p = rp.pick_two();
		// Results should be distinct and from the set {10,20}
		assert((p.first == 10 || p.first == 20));
		assert((p.second == 10 || p.second == 20));
		assert(p.first != p.second);
		// At least one element may remain (10 had count 2)
		assert(rp.size() <= 1);
	},

	// Test 3: adding with non-positive counts should not change picker
	[]() -> void {
		util::random_picker<int> rp;
		rp.add(5, 0);
		rp.add(6, -3);
		assert(rp.size() == 0);
	},

	// Test 4: pick_two on empty picker should throw
	[]() -> void {
		util::random_picker<int> rp;
		bool threw = false;
		try {
			rp.pick_two();
		} catch (const std::out_of_range &e) {
			threw = true;
		}
		assert(threw && "Expected pick_two() to throw on empty random_picker");
	}
};

int main() {
	for (size_t i = 0; i < test_cases.size(); ++i) {
		test_cases[i]();
		std::cout << "Passed test case " << (i + 1) << std::endl;
	}
	std::cout << "All tests passed!" << std::endl;
	return 0;
}
