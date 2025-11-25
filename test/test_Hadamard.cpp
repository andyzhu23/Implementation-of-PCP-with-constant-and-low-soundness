/*
 * Author: Andy Zhu
 * @date    2025-11-25 16:20:00
 * @version 1.0.0
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>

#include "pcpp/Hadamard.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: Empty value vector
    []() -> void {
        std::vector<bool> value = {};
        pcpp::Hadamard h(value);
        assert(h.getCode().size() == 1);
        assert(h.query(0) == false);
    },
    // Test 2: Single true value
    []() -> void {
        std::vector<bool> value = {true};
        pcpp::Hadamard h(value);
        assert(h.getCode().size() == 2);
        assert(h.query(0) == false);
        assert(h.query(1) == true);
    },
    // Test 3: value = {false, true}
    []() -> void {
        std::vector<bool> value = {true, false};
        pcpp::Hadamard h(value);
        std::vector<bool> expected_code = {false, true, false, true};
        assert(h.getCode() == expected_code);
    },
    // Test 4: value = {true, true, false}
    []() -> void {
        std::vector<bool> value = {false, true, true};
        pcpp::Hadamard h(value);
        std::vector<bool> expected_code = {false, false, true, true, true, true, false, false};
        assert(h.getCode() == expected_code);
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << " for Hadamard" << std::endl;
    }
    std::cout << "All Hadamard tests passed!" << std::endl;
    return 0;
}
