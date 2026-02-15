#include <functional>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "pcpp/TesterFactory.hpp"

std::function<int(size_t)> test_iteration_func = [](size_t edge_size) {
    return std::max(static_cast<int>(std::ceil(std::log2(edge_size)) - 5), 0);
};

std::vector<std::function<void(std::ofstream&)>> test_cases = {
    // [](std::ofstream &fout) -> void {
    //     three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 30, 10, 10, 10);
    //     pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
    //     pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
    //     double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
    //     double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
    //     fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    // },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 40, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 50, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 60, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 70, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 80,  10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 90, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 100, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 110, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    [](std::ofstream &fout) -> void {
        three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, 120, 10, 10, 10);
        pcp::BitPCP bitpcp = input.to_BitPCP(pcpp::TesterType::PSEUDO);
        pcp::BitPCP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);
        double original_soundness = analyzer::approximate_soundness(bitpcp, std::min(bitpcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 5, static_cast<size_t>(100000)));
        fout << "original gap: " << 1 - original_soundness << ", amplified gap: " << 1 - amplified_soundness << std::endl;
    },
};


int main() {
    std::ofstream fout("../results/test_three_color_gap_amplification_pseudo_result.txt");
    fout << "Running test_three_color_gap_amplification_pseudo.cpp" << std::endl;
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