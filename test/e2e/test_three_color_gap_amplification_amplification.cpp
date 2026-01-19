#include <functional>
#include <fstream>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"

std::function<int(size_t)> test_iteration_func = [](size_t edge_size) {
    return std::max(static_cast<int>(std::ceil(std::log2(edge_size)) - 5), 0);
};

std::vector<std::function<void(std::ofstream&)>> test_cases = {
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 30, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 40, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 50, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 60, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 70, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 80, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    }, 
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 90, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 100, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 110, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    },
    // Test 3: 
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_invalid_three_coloring_graph(30, 120, 1, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP();
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, test_iteration_func);
        analyzer::PCPAnalyzer analyzer_original({{bitpcp, false}}, 100000);
        analyzer::PCPAnalyzer analyzer_amplified({{amplified_pcp, false}}, 100000);
        fout << "original gap: " << analyzer_original.getGap() << ", amplified gap: " << analyzer_amplified.getGap() << std::endl;
    }
};


int main() {
    std::ofstream fout("test_three_color_gap_amplification_amplification_result.txt");
    fout << "Running test_three_color_gap_amplification_amplification.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        fout << "Running test case: " << (i + 1) << std::endl;
        for (size_t trial = 0; trial < 10; ++trial) {
            fout << " Trial " << (trial + 1) << " ..." << std::endl;
            test_cases[i](fout);
        }
        fout << "Test case " << (i + 1) << " finished\n" << std::endl;
    }
    fout << "All tests finished" << std::endl;
    return 0;
}