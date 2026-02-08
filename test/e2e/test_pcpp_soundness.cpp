#include <cassert>
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "pcpp/Tester.hpp"

std::vector<std::function<void(std::ofstream&)>> test_cases = {
    // Cycle graph, satisfiable
    [](std::ofstream& fout) -> void {
        pcp::BitPCP bitpcp(10);
        for (pcp::Variable i = 0; i < 10; ++i) {
            bitpcp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 10; ++i) {
            bitpcp.add_constraint(i - 1, i, constraint::BitConstraint::EQUAL);
        }
        bitpcp.add_constraint(9, 0, constraint::BitConstraint::EQUAL); // cycle, satisfiable
        pcpp::Tester tc(bitpcp);
        pcp::BitPCP pcpp = tc.buildBitPCP();
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Cycle graph (satisfiable): Approximated original gap: " << 1 - original_soundness << std::endl;
        fout << "Cycle graph (satisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness << std::endl;
        assert(1 - pcpp_soundness == 0);
    },
    // Cycle graph, unsatisfiable
    [](std::ofstream& fout) -> void {
        pcp::BitPCP bitpcp(10);
        for (pcp::Variable i = 0; i < 10; ++i) {
            bitpcp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 10; ++i) {
            bitpcp.add_constraint(i - 1, i, constraint::BitConstraint::EQUAL);
        }
        bitpcp.add_constraint(9, 0, constraint::BitConstraint::NOTEQUAL); // cycle, unsatisfiable
        pcpp::Tester tc(bitpcp);
        pcp::BitPCP pcpp = tc.buildBitPCP();
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Cycle graph (unsatisfiable): Approximated original gap: " << 1 - original_soundness << std::endl;
        fout << "Cycle graph (unsatisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness << std::endl;
        assert(1 - pcpp_soundness > 0);
    },
    // Star graph (center 0)
    [](std::ofstream& fout) -> void {
        pcp::BitPCP bitpcp(10);
        for (pcp::Variable i = 0; i < 10; ++i) {
            bitpcp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 10; ++i) {
            bitpcp.add_constraint(0, i, constraint::BitConstraint::EQUAL);
        }
        pcpp::Tester tc(bitpcp);
        pcp::BitPCP pcpp = tc.buildBitPCP();
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Star graph: Approximated original gap: " << 1 - original_soundness << std::endl;
        fout << "Star graph: Approximated pcpp gap: " << 1 - pcpp_soundness << std::endl;
        assert(1 - pcpp_soundness == 0);
    },
    // Star graph, unsatisfiable with SECOND_BIT_EQUAL and NOTEQUAL constraints
    [](std::ofstream& fout) -> void {
        pcp::BitPCP bitpcp(6);
        for (pcp::Variable i = 0; i < 6; ++i) {
            bitpcp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        // Center 0: alternate constraints
        for (pcp::Variable i = 1; i < 6; ++i) {
            if (i % 2 == 0)
                bitpcp.add_constraint(0, i, constraint::BitConstraint::SECOND_BIT_EQUAL);
            else
                bitpcp.add_constraint(0, i, constraint::BitConstraint::NOTEQUAL);
        }
        // Add a constraint that contradicts the above
        bitpcp.add_constraint(0, 1, constraint::BitConstraint::SECOND_BIT_EQUAL);
        pcpp::Tester tc(bitpcp);
        pcp::BitPCP pcpp = tc.buildBitPCP();
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Star graph (SECOND_BIT_EQUAL/NOTEQUAL, unsatisfiable): Approximated original gap: " << 1 - original_soundness << std::endl;
        fout << "Star graph (SECOND_BIT_EQUAL/NOTEQUAL, unsatisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness << std::endl;
        assert(1 - pcpp_soundness > 0);
    }
};

int main() {
    std::ofstream fout("../results/test_pcpp_soundness_result.txt");
    fout << "Running test_pcpp_soundness.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        fout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i](fout);
    }
    return 0;
}