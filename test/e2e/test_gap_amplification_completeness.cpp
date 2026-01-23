#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"


std::vector<std::function<void()>> test_cases = {
    // Test 1: Small valid three-coloring graph
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(5, 8, 2, 2, 1);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, true}}, 100);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, true}}, 100);
    
        std::cout << "\n=== Test 1: Small Graph ===" << std::endl;
        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;

        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        
        assert(analyzer_amplified.getCompleteness() == 1.0);
    },
    
    // Test 2: Medium valid three-coloring graph
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(10, 15, 4, 3, 3);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, true}}, 100);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, true}}, 100);
    
        std::cout << "\n=== Test 2: Medium Graph ===" << std::endl;
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
    
    // Test 3: Larger valid three-coloring graph
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(15, 25, 5, 5, 5);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, true}}, 100);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, true}}, 100);
    
        std::cout << "\n=== Test 3: Larger Graph ===" << std::endl;
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
    
    // Test 4: Dense graph with higher degree
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(8, 20, 2, 3, 3);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, true}}, 100);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, true}}, 100);
    
        std::cout << "\n=== Test 4: Dense Graph ===" << std::endl;
        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;

        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        
        assert(analyzer_amplified.getCompleteness() == 1.0);
    },
    
    // Test 5: Sparse graph with lower degree
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(12, 12, 4, 4, 4);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, true}}, 100);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, true}}, 100);
    
        std::cout << "\n=== Test 5: Sparse Graph ===" << std::endl;
        std::cout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        std::cout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        std::cout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        std::cout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;

        std::cout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        std::cout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        std::cout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        std::cout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        
        assert(analyzer_amplified.getCompleteness() == 1.0);
    },
    
};

int main() {
    std::cout << "Running test_gap_amplification_completeness.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i]();
    }
    return 0;
}