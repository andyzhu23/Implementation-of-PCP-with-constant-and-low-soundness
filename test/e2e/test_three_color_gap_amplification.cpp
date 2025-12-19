#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/three_color.hpp"
#include "analyzer/PCPAnalyzer.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: Random invalid three-coloring graphs
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(10, 30, 1, 3, 3, 4);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 10000);

        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;


        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 10000);
    
        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;

        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Test 2: Larger random invalid three-coloring graphs
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(100, 500, 1, 40, 30, 30);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;


        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
    
        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    }
};

int main() {
    std::cout << "Running test_soundness_amplification.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i]();
    }
    return 0;
}