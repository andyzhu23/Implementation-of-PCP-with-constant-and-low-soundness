#include <cassert>
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>

#include "core/core.hpp"
#include "three_color/ThreeColor.hpp"
#include "analyzer/PCPAnalyzer.hpp"
#include "analyzer/SoundnessApproximater.hpp"
#include "pcpp/HadamardPCPP/HadamardTester.hpp"
#include "util/disjoint_set_union.hpp"

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
        pcpp::HadamardTester hadamard_tester(bitpcp);
        hadamard_tester.create_tester(bitpcp);
        pcp::BitPCP pcpp = hadamard_tester.buildBitPCP(100, 100, 100);
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
        pcpp::HadamardTester hadamard_tester(bitpcp);
        hadamard_tester.create_tester(bitpcp);
        pcp::BitPCP pcpp = hadamard_tester.buildBitPCP(100, 100, 100);
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
        pcpp::HadamardTester hadamard_tester(bitpcp);
        hadamard_tester.create_tester(bitpcp);
        pcp::BitPCP pcpp = hadamard_tester.buildBitPCP(100, 100, 100);
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
        pcpp::HadamardTester hadamard_tester(bitpcp);
        hadamard_tester.create_tester(bitpcp);
        pcp::BitPCP pcpp = hadamard_tester.buildBitPCP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(bitpcp);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Star graph (SECOND_BIT_EQUAL/NOTEQUAL, unsatisfiable): Approximated original gap: " << 1 - original_soundness << std::endl;
        fout << "Star graph (SECOND_BIT_EQUAL/NOTEQUAL, unsatisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness << std::endl;
        assert(1 - pcpp_soundness > 0);
    }, 
    // Ring graph with four nodes multiple local views encoded into pcpp, globally satisfiable, check that satisfiability is preserved
    [](std::ofstream& fout) -> void {
        // variables 0, 1
        pcp::BitPCP bitpcp1(2);
        bitpcp1.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // variables 1, 2
        pcp::BitPCP bitpcp2(2);
        bitpcp2.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // variables 2, 3
        pcp::BitPCP bitpcp3(2);
        bitpcp3.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // variables 3, 0
        pcp::BitPCP bitpcp4(2);
        bitpcp4.add_constraint(0, 1, constraint::BitConstraint::EQUAL);

        pcpp::HadamardTester hadamard_tester(bitpcp1);
        hadamard_tester.create_tester(bitpcp1);
        pcp::BitPCP pcpp1 = hadamard_tester.buildBitPCP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(bitpcp2);
        hadamard_tester.create_tester(bitpcp2);
        pcp::BitPCP pcpp2 = hadamard_tester.buildBitPCP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(bitpcp3);
        hadamard_tester.create_tester(bitpcp3);
        pcp::BitPCP pcpp3 = hadamard_tester.buildBitPCP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(bitpcp4);
        hadamard_tester.create_tester(bitpcp4);
        pcp::BitPCP pcpp4 = hadamard_tester.buildBitPCP(30, 30, 30);
        std::vector<size_t> offsets = {
            0, 
            pcpp1.get_size(), pcpp1.get_size() + pcpp2.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size() + pcpp4.get_size()
        };

        pcp::BitPCP combined_bitpcp = pcp::merge_BitPCPs(std::vector<pcp::BitPCP>{pcpp1, pcpp2, pcpp3, pcpp4});

        util::disjoint_set_union dsu(offsets.back());


        // pcpp1 variable 1 corresponds to pcpp2 variable 0 (both represent global variable 1)
        dsu.merge(offsets[0] + 3, offsets[1]);
        dsu.merge(offsets[0] + 3 + 1, offsets[1] + 1);
        dsu.merge(offsets[0] + 3 + 2, offsets[1] + 2);

        // pcpp2 variable 1 corresponds to pcpp3 variable 0 (both represent global variable 2)
        dsu.merge(offsets[1] + 3, offsets[2]);
        dsu.merge(offsets[1] + 3 + 1, offsets[2] + 1);
        dsu.merge(offsets[1] + 3 + 2, offsets[2] + 2);
        
        // pcpp3 variable 1 corresponds to pcpp4 variable 0 (both represent global variable 3)
        dsu.merge(offsets[2] + 3, offsets[3]);
        dsu.merge(offsets[2] + 3 + 1, offsets[3] + 1);
        dsu.merge(offsets[2] + 3 + 2, offsets[3] + 2);
        
        // pcpp4 variable 1 corresponds to pcpp1 variable 0 (both represent global variable 0)
        dsu.merge(offsets[3] + 3, offsets[0]);
        dsu.merge(offsets[3] + 3 + 1, offsets[0] + 1);
        dsu.merge(offsets[3] + 3 + 2, offsets[0] + 2);

        std::unordered_map<pcp::Variable, pcp::Variable> representative_map;
        size_t new_size = 0;
        for (pcp::Variable i = 0; i < offsets.back(); ++i) {
            if (dsu.find(i) == i) {
                representative_map[i] = new_size++;
            }
        }

        pcp::BitPCP new_bitpcp(new_size);

        for (pcp::Variable i = 0; i < offsets.back(); ++i) {
            if (dsu.find(i) == i) {
                new_bitpcp.set_variable(
                    representative_map[i],
                    combined_bitpcp.get_variable(i)
                );
            }
        }

        for (const auto &[u, v, c] : combined_bitpcp.get_constraints_list()) {
            pcp::Variable new_u = representative_map[dsu.find(u)];
            pcp::Variable new_v = representative_map[dsu.find(v)];
            new_bitpcp.add_constraint(new_u, new_v, c);
        }

        double merged_gap = 1 - analyzer::approximate_soundness(new_bitpcp, 3000000);
        fout << "Alphabet reduced PCP(Satisfiable) gap: " << merged_gap << std::endl;
        assert(merged_gap == 0);
    },
    // Ring graph with four nodes, multiple local views encoded into pcpp, globally unsatisfiable, check that unsatisfiability is preserved
    [](std::ofstream& fout) -> void {
        // variables 0, 1
        pcp::BitPCP bitpcp1(2);
        bitpcp1.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // variables 1, 2
        pcp::BitPCP bitpcp2(2);
        bitpcp2.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // variables 2, 3
        pcp::BitPCP bitpcp3(2);
        bitpcp3.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        // variables 3, 1
        pcp::BitPCP bitpcp4(2);
        bitpcp4.add_constraint(0, 1, constraint::BitConstraint::NOTEQUAL);

        pcpp::HadamardTester hadamard_tester(bitpcp1);
        hadamard_tester.create_tester(bitpcp1);
        pcp::BitPCP pcpp1 = hadamard_tester.buildBitPCP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(bitpcp2);
        hadamard_tester.create_tester(bitpcp2);
        pcp::BitPCP pcpp2 = hadamard_tester.buildBitPCP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(bitpcp3);
        hadamard_tester.create_tester(bitpcp3);
        pcp::BitPCP pcpp3 = hadamard_tester.buildBitPCP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(bitpcp4);
        hadamard_tester.create_tester(bitpcp4);
        pcp::BitPCP pcpp4 = hadamard_tester.buildBitPCP(30, 30, 30);

        std::vector<size_t> offsets = {
            0, 
            pcpp1.get_size(), pcpp1.get_size() + pcpp2.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size() + pcpp4.get_size()
        };

        pcp::BitPCP combined_bitpcp = pcp::merge_BitPCPs(std::vector<pcp::BitPCP>{pcpp1, pcpp2, pcpp3, pcpp4});

        util::disjoint_set_union dsu(offsets.back());

        // pcpp1 variable 1 corresponds to pcpp2 variable 0 (both represent global variable 1)
        dsu.merge(offsets[0] + 3, offsets[1]);
        dsu.merge(offsets[0] + 3 + 1, offsets[1] + 1);
        dsu.merge(offsets[0] + 3 + 2, offsets[1] + 2);
        
        // pcpp2 variable 1 corresponds to pcpp3 variable 0 (both represent global variable 2)
        dsu.merge(offsets[1] + 3, offsets[2]);
        dsu.merge(offsets[1] + 3 + 1, offsets[2] + 1);
        dsu.merge(offsets[1] + 3 + 2, offsets[2] + 2);
        
        // pcpp3 variable 1 corresponds to pcpp4 variable 0 (both represent global variable 3)
        dsu.merge(offsets[2] + 3, offsets[3]);
        dsu.merge(offsets[2] + 3 + 1, offsets[3] + 1);
        dsu.merge(offsets[2] + 3 + 2, offsets[3] + 2);
        
        // pcpp4 variable 1 corresponds to pcpp1 variable 0 (both represent global variable 0)
        dsu.merge(offsets[3] + 3, offsets[0]);
        dsu.merge(offsets[3] + 3 + 1, offsets[0] + 1);
        dsu.merge(offsets[3] + 3 + 2, offsets[0] + 2);

        std::unordered_map<pcp::Variable, pcp::Variable> representative_map;
        size_t new_size = 0;
        for (pcp::Variable i = 0; i < offsets.back(); ++i) {
            if (dsu.find(i) == i) {
                representative_map[i] = new_size++;
            }
        }

        pcp::BitPCP new_bitpcp(new_size);

        for (pcp::Variable i = 0; i < offsets.back(); ++i) {
            if (dsu.find(i) == i) {
                new_bitpcp.set_variable(
                    representative_map[i],
                    combined_bitpcp.get_variable(i)
                );
            }
        }

        for (const auto &[u, v, c] : combined_bitpcp.get_constraints_list()) {
            pcp::Variable new_u = representative_map[dsu.find(u)];
            pcp::Variable new_v = representative_map[dsu.find(v)];
            new_bitpcp.add_constraint(new_u, new_v, c);
        }

        double merged_gap = 1 - analyzer::approximate_soundness(new_bitpcp, 3000000);
        fout << "Alphabet reduced PCP(UnSatisfiable) gap: " << merged_gap << std::endl;
        assert(merged_gap > 0);
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