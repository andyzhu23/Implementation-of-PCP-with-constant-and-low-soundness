#include <functional>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "analyzer/PCPAnalyzer.hpp"
#include "pcpp/TesterFactory.hpp"

std::function<int(size_t)> test_iteration_func = [](size_t edge_size) {
    return std::max(static_cast<int>(std::ceil(std::log2(edge_size)) - 5), 0);
};

void run_test_case(std::ofstream &fout, int edge_count) {
    three_color::ThreeColor input = three_color::generate_non_three_colorable_graph(30, edge_count, 10, 10, 10);
    pcp::BinaryCSP BinaryCSP = input.to_BinaryCSP(pcpp::TesterType::PSEUDO);
    pcp::BinaryCSP amplified_pcp = core::three_color_gap_amplification(input, pcpp::TesterType::PSEUDO, test_iteration_func);

    double original_soundness = analyzer::approximate_soundness(BinaryCSP, std::min(BinaryCSP.get_constraints_list().size() * 10, static_cast<size_t>(100000)));
    double amplified_soundness = analyzer::approximate_soundness(amplified_pcp, std::min(amplified_pcp.get_constraints_list().size() * 10, static_cast<size_t>(100000)));

    fout << "original gap: " << 1 - original_soundness
         << ", amplified gap: " << 1 - amplified_soundness
         << ", original number of variables: " << BinaryCSP.get_size()
         << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
         << ", new number of variables: " << amplified_pcp.get_size()
         << ", new number of constraints: " << amplified_pcp.get_constraints_list().size()
         << std::endl;
}

std::vector<std::function<void(std::ofstream&)>> test_cases = {
    [](std::ofstream &fout) -> void { run_test_case(fout, 30); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 40); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 50); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 60); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 70); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 80); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 90); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 100); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 110); },
    [](std::ofstream &fout) -> void { run_test_case(fout, 120); },
};


int main() {
    std::ofstream fout("../results/test_three_color_gap_amplification_pseudo_result.txt");
    fout << "Running test_three_color_gap_amplification_pseudo.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        fout << "Running test case: " << (i + 1) << std::endl;
        for (size_t trial = 0; trial < 30; ++trial) {
            fout << " Trial " << (trial + 1) << " ..." << std::endl;
            test_cases[i](fout);
        }
        fout << "Test case " << (i + 1) << " finished\n" << std::endl;
    }
    fout << "All tests finished" << std::endl;
    return 0;
}