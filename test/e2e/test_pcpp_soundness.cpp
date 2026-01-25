#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "pcpp/Tester.hpp"

std::vector<std::function<void()>> test_cases = {
    // // Test 1: Simple non-three-colorable graph
    // []() -> void {
    //     three_color::ThreeColor input({three_color::Color::RED, three_color::Color::RED, three_color::Color::RED, three_color::Color::RED}, {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {2, 3}});
    //     pcp::BitPCP bitpcp = input.to_BitPCP();
    //     pcp::BitPCP amplified_pcp = core::gap_amplification(bitpcp);
    //     double original_soundness = analyzer::approximate_soundness(bitpcp);
    //     double amplified_soundness = analyzer::approximate_soundness(amplified_pcp);
    //     std::cout << "Approximated original gap: " << 1 - original_soundness << std::endl;
        
    //     std::cout << "Approximated amplified gap: " << 1 - amplified_soundness << std::endl;
    // },
    // Test 1: Simple non-satisfiable CSP
    // []() -> void {
    //     pcp::BitPCP bitpcp(2);
    //     bitpcp.add_constraint(1, 0, constraint::BitConstraint::EQUAL); 
    //     bitpcp.add_constraint(1, 0, constraint::BitConstraint::NOTEQUAL); // make it unsatisfiable

    //     pcpp::Tester tester(bitpcp);
    //     pcp::BitPCP amplified_pcp = tester.buildBitPCP();

    //     double original_soundness = analyzer::approximate_soundness(bitpcp);
    //     double amplified_soundness = analyzer::approximate_soundness(amplified_pcp);
    //     std::cout << "Approximated original gap: " << 1 - original_soundness << std::endl;
        
    //     std::cout << "Approximated amplified gap: " << 1 - amplified_soundness << std::endl;
    // },
    // Test 2: Simple non-satisfiable CSP
    []() -> void {
        pcp::BitPCP bitpcp(100);
        for (pcp::Variable i = 0; i < 100; ++i) {
            bitpcp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 100; ++i) {
            bitpcp.add_constraint(i - 1, i, constraint::BitConstraint::EQUAL);
        }
        bitpcp.add_constraint(99, 0, constraint::BitConstraint::EQUAL); // make it satisfiable
        pcpp::Tester tc(bitpcp);
        pcp::BitPCP amplified_pcp = tc.buildBitPCP();
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp);
        std::cout << "Approximated original gap: " << 1 - original_soundness << std::endl;
        
        std::cout << "Approximated amplified gap: " << 1 - amplified_soundness << std::endl;
    },
    // Test 2: Simple non-satisfiable CSP
    []() -> void {
        pcp::BitPCP bitpcp(100);
        for (pcp::Variable i = 0; i < 100; ++i) {
            bitpcp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 100; ++i) {
            bitpcp.add_constraint(i - 1, i, constraint::BitConstraint::EQUAL);
        }
        bitpcp.add_constraint(99, 0, constraint::BitConstraint::NOTEQUAL); // make it unsatisfiable

        pcpp::Tester tc(bitpcp);
        pcp::BitPCP amplified_pcp = tc.buildBitPCP();
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double amplified_soundness = analyzer::approximate_soundness(amplified_pcp);
        std::cout << "Approximated original gap: " << 1 - original_soundness << std::endl;
        
        std::cout << "Approximated amplified gap: " << 1 - amplified_soundness << std::endl;
    }
};

int main() {
    std::cout << "Running test_pcpp_soundness.cpp" << std::endl;
    for (size_t i = 0; i < test_cases.size(); ++i) {
        std::cout << "Running test case: " << (i + 1) << std::endl;
        test_cases[i]();
    }
    return 0;
}