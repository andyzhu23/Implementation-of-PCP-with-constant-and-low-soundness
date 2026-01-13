#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: Simple triangle graph (invalid coloring)
    []()-> void {
        std::vector<three_color::Color> colors = { three_color::Color::RED, three_color::Color::GREEN, three_color::Color::GREEN };
        std::vector<three_color::Edge> edges = { {0,1}, {1,2}, {2,0} };
        three_color::ThreeColor tc(colors, edges);
        auto bitpcp = tc.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 10000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 10000);
        
        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;
        
        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;

        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Test 2: Random valid three-coloring graphs
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(10, 15, 4, 3, 3);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, true}}, 100);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, true}}, 100);
    
        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;

        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
        assert(analyzer_amplified.getCompleteness() == 1.0);
    }, 
    // Test 3: Random invalid three-coloring graphs
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(10, 30, 1, 3, 3, 4);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
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