#include <cassert>
#include <functional>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/three_color.hpp"
#include "analyzer/PCPAnalyzer.hpp"

std::vector<std::function<void(std::ofstream&)>> test_cases = {
    // Test 1: Random invalid three-coloring graphs
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(10, 30, 1, 3, 3, 4);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 10000);

        fout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        fout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        fout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        fout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;


        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 10000);
    
        fout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        fout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        fout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        fout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;

        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Test 2: Larger random invalid three-coloring graphs
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(50, 200, 1, 20, 15, 15);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        fout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        fout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        fout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        fout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;


        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
    
        fout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        fout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        fout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        fout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Test 3: random invalid three-coloring graphs with uneven coloring distribution
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(50, 100, 1, 48, 1, 1);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        fout << "Original PCP size: " << bitpcp.get_size() << std::endl;
        fout << "Original PCP completeness: " << analyzer_original.getCompleteness() << std::endl;
        fout << "Original PCP soundness: " << analyzer_original.getSoundness() << std::endl;
        fout << "Original PCP gap: " << analyzer_original.getGap() << std::endl;


        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
    
        fout << "Amplified PCP size: " << amplified_pcp.get_size() << std::endl;
        fout << "Amplified PCP completeness: " << analyzer_amplified.getCompleteness() << std::endl;
        fout << "Amplified PCP soundness: " << analyzer_amplified.getSoundness() << std::endl;
        fout << "Amplified PCP gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Additional Test 4: Small triangle (3-node, 3-edge) with one violated edge
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(3, 3, 1, 1, 1, 1);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 1000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 1000);
        fout << "Triangle - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Additional Test 5: Sparse graph approximating a tree: n nodes, n-1 edges, few violated edges
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(20, 19, 2, 7, 7, 6);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 20000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 20000);
        fout << "Tree-like - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Additional Test 6: Cycle graph: n nodes, n edges, introduce a few violated edges
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 30, 5, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 30000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 30000);
        fout << "Cycle-like - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Additional Test 7: Small complete graph (dense): choose n=6 (15 edges), force multiple violations
    [](std::ofstream &fout) -> void {
        size_t n = 6;
        size_t edges = n * (n - 1) / 2;
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(n, edges, 6, 2, 2, 2);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 50000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 50000);
        fout << "Complete-small - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    },
    // Additional Test 8: Valid three-colorable graph - should have high completeness; amplification should not reduce gap
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_valid_three_coloring_graph(40, 120, 13, 13, 14);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 40000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 40000);
        fout << "Valid-graph - original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
        assert(analyzer_amplified.getGap() >= analyzer_original.getGap());
    }
};


int main() {
    std::ofstream fout("test_three_color_gap_amplification_result.txt");
    fout << "Running test_soundness_amplification.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        fout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i](fout);
        fout.flush();
    }
    fout << "All tests finished" << std::endl;
    return 0;
}