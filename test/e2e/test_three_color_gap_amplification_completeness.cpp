#include <cassert>
#include <functional>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"

std::vector<std::function<void()>> test_cases = {
    // Test 1: Random invalid three-coloring graphs
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(50, 99, 1, 20, 20, 10);
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
    },
    // Test 2: Larger random invalid three-coloring graphs
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(50, 200, 1, 20, 15, 15);
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
    },
    // Test 3: random invalid three-coloring graphs with uneven coloring distribution
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(50, 100, 1, 46, 2, 2);
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
    },
    // Additional Test 4: Sparse graph approximating a tree: n nodes, n-1 edges, few violated edges
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(20, 19, 2, 7, 7, 6);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 20000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 20000);
        std::cout << "Tree-like - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    // Additional Test 5: Cycle graph: n nodes, n edges, introduce a few violated edges
    []() -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 30, 5, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 30000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 30000);
        std::cout << "Cycle-like - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    // Additional Test 6: Small complete graph (dense): choose n=6 (15 edges), force multiple violations
    []() -> void {
        size_t n = 6;
        size_t edges = n * (n - 1) / 2;
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(n, edges, 6, 2, 2, 2);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 50000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 50000);
        std::cout << "Complete-small - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    // Additional Test 7: Valid three-colorable graph - should have completeness; amplification should not reduce gap
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(40, 120, 13, 13, 14);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 40000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 40000);
        std::cout << "Valid-graph - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() == 0);
    }, 
    // Additional Test 8: Valid three-colorable graph - should have completeness; amplification should not reduce gap
    []() -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(40, 500, 13, 13, 14);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 40000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 40000);
        std::cout << "Valid-graph - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() == 0);
    }, 
};


int main() {
    std::cout << "Running test_three_color_gap_amplification_completeness.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i]();
        std::cout.flush();
    }
    std::cout << "All tests finished" << std::endl;
    return 0;
}