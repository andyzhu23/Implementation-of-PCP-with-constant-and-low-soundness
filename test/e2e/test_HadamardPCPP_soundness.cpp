#include <cassert>
#include <functional>
#include <iostream>
#include <fstream>
#include <random>
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
        pcp::BinaryCSP BinaryCSP(10);
        for (pcp::Variable i = 0; i < 10; ++i) {
            BinaryCSP.set_variable(i, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 10; ++i) {
            BinaryCSP.add_constraint(i - 1, i, constraint::BinaryConstraint::EQUAL);
        }
        BinaryCSP.add_constraint(9, 0, constraint::BinaryConstraint::EQUAL); // cycle, satisfiable
        pcpp::HadamardTester hadamard_tester(BinaryCSP);
        hadamard_tester.create_tester(BinaryCSP);
        pcp::BinaryCSP pcpp = hadamard_tester.buildBinaryCSP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(BinaryCSP);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
           fout << "Cycle graph (satisfiable): Approximated original gap: " << 1 - original_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
           fout << "Cycle graph (satisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
        assert(1 - pcpp_soundness == 0);
    },
    // Cycle graph, unsatisfiable
    [](std::ofstream& fout) -> void {
        pcp::BinaryCSP BinaryCSP(10);
        for (pcp::Variable i = 0; i < 10; ++i) {
            BinaryCSP.set_variable(i, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 10; ++i) {
            BinaryCSP.add_constraint(i - 1, i, constraint::BinaryConstraint::EQUAL);
        }
        BinaryCSP.add_constraint(9, 0, constraint::BinaryConstraint::NOTEQUAL); // cycle, unsatisfiable
        pcpp::HadamardTester hadamard_tester(BinaryCSP);
        hadamard_tester.create_tester(BinaryCSP);
        pcp::BinaryCSP pcpp = hadamard_tester.buildBinaryCSP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(BinaryCSP);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
           fout << "Cycle graph (unsatisfiable): Approximated original gap: " << 1 - original_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
           fout << "Cycle graph (unsatisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
        assert(1 - pcpp_soundness > 0);
    },
    // Star graph (center 0)
    [](std::ofstream& fout) -> void {
        pcp::BinaryCSP BinaryCSP(10);
        for (pcp::Variable i = 0; i < 10; ++i) {
            BinaryCSP.set_variable(i, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        for (pcp::Variable i = 1; i < 10; ++i) {
            BinaryCSP.add_constraint(0, i, constraint::BinaryConstraint::EQUAL);
        }
        pcpp::HadamardTester hadamard_tester(BinaryCSP);
        hadamard_tester.create_tester(BinaryCSP);
        pcp::BinaryCSP pcpp = hadamard_tester.buildBinaryCSP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(BinaryCSP);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
           fout << "Star graph: Approximated original gap: " << 1 - original_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
           fout << "Star graph: Approximated pcpp gap: " << 1 - pcpp_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
        assert(1 - pcpp_soundness == 0);
    },
    // Star graph, unsatisfiable with SECOND_BIT_EQUAL and NOTEQUAL constraints
    [](std::ofstream& fout) -> void {
        pcp::BinaryCSP BinaryCSP(6);
        for (pcp::Variable i = 0; i < 6; ++i) {
            BinaryCSP.set_variable(i, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }
        // Center 0: alternate constraints
        for (pcp::Variable i = 1; i < 6; ++i) {
            if (i % 2 == 0)
                BinaryCSP.add_constraint(0, i, constraint::BinaryConstraint::SECOND_BIT_EQUAL);
            else
                BinaryCSP.add_constraint(0, i, constraint::BinaryConstraint::NOTEQUAL);
        }
        // Add a constraint that contradicts the above
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::SECOND_BIT_EQUAL);
        pcpp::HadamardTester hadamard_tester(BinaryCSP);
        hadamard_tester.create_tester(BinaryCSP);
        pcp::BinaryCSP pcpp = hadamard_tester.buildBinaryCSP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(BinaryCSP);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
           fout << "Star graph (SECOND_BIT_EQUAL/NOTEQUAL, unsatisfiable): Approximated original gap: " << 1 - original_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
           fout << "Star graph (SECOND_BIT_EQUAL/NOTEQUAL, unsatisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness
               << ", original number of variables: " << BinaryCSP.get_size()
               << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << pcpp.get_size()
               << ", new number of constraints: " << pcpp.get_constraints_list().size()
               << std::endl;
        assert(1 - pcpp_soundness > 0);
    },

    // Random graph with 10 variables and 10 constraints, satisfiable by construction
    [](std::ofstream& fout) -> void {
        constexpr pcp::Variable n = 10;
        constexpr int m = 10;
        std::mt19937 rng(20260406);
        std::uniform_int_distribution<int> bit_dist(0, 1);
        std::uniform_int_distribution<int> var_dist(0, n - 1);

        std::vector<int> assignment(n, 0);
        for (pcp::Variable i = 0; i < n; ++i) {
            assignment[i] = bit_dist(rng);
        }

        pcp::BinaryCSP BinaryCSP(n);
        for (pcp::Variable i = 0; i < n; ++i) {
            BinaryCSP.set_variable(i, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }

        for (int e = 0; e < m; ++e) {
            pcp::Variable u = var_dist(rng);
            pcp::Variable v = var_dist(rng);
            while (v == u) {
                v = var_dist(rng);
            }
            auto c = (assignment[u] == assignment[v])
                ? constraint::BinaryConstraint::EQUAL
                : constraint::BinaryConstraint::NOTEQUAL;
            BinaryCSP.add_constraint(u, v, c);
        }

        pcpp::HadamardTester hadamard_tester(BinaryCSP);
        hadamard_tester.create_tester(BinaryCSP);
        pcp::BinaryCSP pcpp = hadamard_tester.buildBinaryCSP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(BinaryCSP);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Random graph (n=10, m=10, satisfiable): Approximated original gap: " << 1 - original_soundness
             << ", original number of variables: " << BinaryCSP.get_size()
             << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
             << ", new number of variables: " << pcpp.get_size()
             << ", new number of constraints: " << pcpp.get_constraints_list().size()
             << std::endl;
        fout << "Random graph (n=10, m=10, satisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness
             << ", original number of variables: " << BinaryCSP.get_size()
             << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
             << ", new number of variables: " << pcpp.get_size()
             << ", new number of constraints: " << pcpp.get_constraints_list().size()
             << std::endl;
        assert(1 - pcpp_soundness == 0);
    },

    // Random graph with 10 variables and 10 constraints, unsatisfiable by construction
    [](std::ofstream& fout) -> void {
        constexpr pcp::Variable n = 10;
        constexpr int m = 10;
        std::mt19937 rng(20260407);
        std::uniform_int_distribution<int> bit_dist(0, 1);
        std::uniform_int_distribution<int> var_dist(0, n - 1);

        std::vector<int> assignment(n, 0);
        for (pcp::Variable i = 0; i < n; ++i) {
            assignment[i] = bit_dist(rng);
        }

        pcp::BinaryCSP BinaryCSP(n);
        for (pcp::Variable i = 0; i < n; ++i) {
            BinaryCSP.set_variable(i, pcp::BinaryDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
        }

        // Contradiction on the same pair guarantees unsatisfiability.
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        BinaryCSP.add_constraint(0, 1, constraint::BinaryConstraint::NOTEQUAL);

        for (int e = 2; e < m; ++e) {
            pcp::Variable u = var_dist(rng);
            pcp::Variable v = var_dist(rng);
            while (v == u) {
                v = var_dist(rng);
            }
            auto c = (assignment[u] == assignment[v])
                ? constraint::BinaryConstraint::EQUAL
                : constraint::BinaryConstraint::NOTEQUAL;
            BinaryCSP.add_constraint(u, v, c);
        }

        pcpp::HadamardTester hadamard_tester(BinaryCSP);
        hadamard_tester.create_tester(BinaryCSP);
        pcp::BinaryCSP pcpp = hadamard_tester.buildBinaryCSP(100, 100, 100);
        double original_soundness = analyzer::approximate_soundness(BinaryCSP);
        double pcpp_soundness = analyzer::approximate_soundness(pcpp);
        fout << "Random graph (n=10, m=10, unsatisfiable): Approximated original gap: " << 1 - original_soundness
             << ", original number of variables: " << BinaryCSP.get_size()
             << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
             << ", new number of variables: " << pcpp.get_size()
             << ", new number of constraints: " << pcpp.get_constraints_list().size()
             << std::endl;
        fout << "Random graph (n=10, m=10, unsatisfiable): Approximated pcpp gap: " << 1 - pcpp_soundness
             << ", original number of variables: " << BinaryCSP.get_size()
             << ", original number of constraints: " << BinaryCSP.get_constraints_list().size()
             << ", new number of variables: " << pcpp.get_size()
             << ", new number of constraints: " << pcpp.get_constraints_list().size()
             << std::endl;
        assert(1 - pcpp_soundness > 0);
    }, 
    
    // Ring graph with four nodes multiple local views encoded into pcpp, globally satisfiable, check that satisfiability is preserved
    [](std::ofstream& fout) -> void {
        // variables 0, 1
        pcp::BinaryCSP BinaryCSP1(2);
        BinaryCSP1.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // variables 1, 2
        pcp::BinaryCSP BinaryCSP2(2);
        BinaryCSP2.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // variables 2, 3
        pcp::BinaryCSP BinaryCSP3(2);
        BinaryCSP3.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // variables 3, 0
        pcp::BinaryCSP BinaryCSP4(2);
        BinaryCSP4.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);

        pcpp::HadamardTester hadamard_tester(BinaryCSP1);
        hadamard_tester.create_tester(BinaryCSP1);
        pcp::BinaryCSP pcpp1 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(BinaryCSP2);
        hadamard_tester.create_tester(BinaryCSP2);
        pcp::BinaryCSP pcpp2 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(BinaryCSP3);
        hadamard_tester.create_tester(BinaryCSP3);
        pcp::BinaryCSP pcpp3 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(BinaryCSP4);
        hadamard_tester.create_tester(BinaryCSP4);
        pcp::BinaryCSP pcpp4 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        std::vector<size_t> offsets = {
            0, 
            pcpp1.get_size(), pcpp1.get_size() + pcpp2.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size() + pcpp4.get_size()
        };

        pcp::BinaryCSP combined_BinaryCSP = pcp::merge_BinaryCSPs(std::vector<pcp::BinaryCSP>{pcpp1, pcpp2, pcpp3, pcpp4});

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

        pcp::BinaryCSP new_BinaryCSP(new_size);

        for (pcp::Variable i = 0; i < offsets.back(); ++i) {
            if (dsu.find(i) == i) {
                new_BinaryCSP.set_variable(
                    representative_map[i],
                    combined_BinaryCSP.get_variable(i)
                );
            }
        }

        for (const auto &[u, v, c] : combined_BinaryCSP.get_constraints_list()) {
            pcp::Variable new_u = representative_map[dsu.find(u)];
            pcp::Variable new_v = representative_map[dsu.find(v)];
            new_BinaryCSP.add_constraint(new_u, new_v, c);
        }

        double merged_gap = 1 - analyzer::approximate_soundness(new_BinaryCSP, 3000000);
           fout << "Alphabet reduced PCP(Satisfiable) gap: " << merged_gap
               << ", original number of variables: " << combined_BinaryCSP.get_size()
               << ", original number of constraints: " << combined_BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << new_BinaryCSP.get_size()
               << ", new number of constraints: " << new_BinaryCSP.get_constraints_list().size()
               << std::endl;
        assert(merged_gap == 0);
    },
    // Ring graph with four nodes, multiple local views encoded into pcpp, globally unsatisfiable, check that unsatisfiability is preserved
    [](std::ofstream& fout) -> void {
        // variables 0, 1
        pcp::BinaryCSP BinaryCSP1(2);
        BinaryCSP1.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // variables 1, 2
        pcp::BinaryCSP BinaryCSP2(2);
        BinaryCSP2.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // variables 2, 3
        pcp::BinaryCSP BinaryCSP3(2);
        BinaryCSP3.add_constraint(0, 1, constraint::BinaryConstraint::EQUAL);
        // variables 3, 1
        pcp::BinaryCSP BinaryCSP4(2);
        BinaryCSP4.add_constraint(0, 1, constraint::BinaryConstraint::NOTEQUAL);

        pcpp::HadamardTester hadamard_tester(BinaryCSP1);
        hadamard_tester.create_tester(BinaryCSP1);
        pcp::BinaryCSP pcpp1 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(BinaryCSP2);
        hadamard_tester.create_tester(BinaryCSP2);
        pcp::BinaryCSP pcpp2 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(BinaryCSP3);
        hadamard_tester.create_tester(BinaryCSP3);
        pcp::BinaryCSP pcpp3 = hadamard_tester.buildBinaryCSP(30, 30, 30);
        hadamard_tester = pcpp::HadamardTester(BinaryCSP4);
        hadamard_tester.create_tester(BinaryCSP4);
        pcp::BinaryCSP pcpp4 = hadamard_tester.buildBinaryCSP(30, 30, 30);

        std::vector<size_t> offsets = {
            0, 
            pcpp1.get_size(), pcpp1.get_size() + pcpp2.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size(), 
            pcpp1.get_size() + pcpp2.get_size() + pcpp3.get_size() + pcpp4.get_size()
        };

        pcp::BinaryCSP combined_BinaryCSP = pcp::merge_BinaryCSPs(std::vector<pcp::BinaryCSP>{pcpp1, pcpp2, pcpp3, pcpp4});

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

        pcp::BinaryCSP new_BinaryCSP(new_size);

        for (pcp::Variable i = 0; i < offsets.back(); ++i) {
            if (dsu.find(i) == i) {
                new_BinaryCSP.set_variable(
                    representative_map[i],
                    combined_BinaryCSP.get_variable(i)
                );
            }
        }

        for (const auto &[u, v, c] : combined_BinaryCSP.get_constraints_list()) {
            pcp::Variable new_u = representative_map[dsu.find(u)];
            pcp::Variable new_v = representative_map[dsu.find(v)];
            new_BinaryCSP.add_constraint(new_u, new_v, c);
        }

        double merged_gap = 1 - analyzer::approximate_soundness(new_BinaryCSP, 3000000);
           fout << "Alphabet reduced PCP(UnSatisfiable) gap: " << merged_gap
               << ", original number of variables: " << combined_BinaryCSP.get_size()
               << ", original number of constraints: " << combined_BinaryCSP.get_constraints_list().size()
               << ", new number of variables: " << new_BinaryCSP.get_size()
               << ", new number of constraints: " << new_BinaryCSP.get_constraints_list().size()
               << std::endl;
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