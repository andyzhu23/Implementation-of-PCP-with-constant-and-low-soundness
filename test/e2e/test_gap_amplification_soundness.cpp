#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"
#include "analyzer/SoundnessApproximater.hpp"


std::vector<std::function<void()>> test_cases = {
    []() -> void {
        three_color::ThreeColor input({three_color::Color::RED, three_color::Color::RED, three_color::Color::RED, three_color::Color::RED}, {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {2, 3}});
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        double soundness = analyzer::approximate_soundness(amplified_pcp);
        std::cout << "Approximated soundness: " << soundness << std::endl;
    }
};

int main() {
    std::cout << "Running test_gap_amplification_soundness.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i]();
    }
    return 0;
}