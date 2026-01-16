#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "analyzer/PCPAnalyzer.hpp"
#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"

std::vector<std::function<void()>> test_cases = {
    []() -> void {
        // Test case 1: Simple PCP with no constraints
        pcp::BitPCP pcp1(3);  // 3 variables, no constraints
        std::vector<std::pair<pcp::BitPCP, analyzer::Satisfiability>> samples = {
            {pcp1, true},
        };
        analyzer::PCPAnalyzer analyzer(samples, 10);
        double soundness = analyzer.getSoundness();
        double completeness = analyzer.getCompleteness();
        double gap = analyzer.getGap();

        // Check if values make sense
        assert(completeness == 1.0);  // Completeness should always be 1
        assert(soundness == 0.0);     // No unsatisfiable samples are accepted
        assert(gap == 1.0);          // Maximum gap
    },
    []() -> void {
        // Test case 2: Simple PCP with one constraint
        pcp::BitPCP pcp2_1(std::move(std::vector<pcp::BitDomain>{0, 0}));
        pcp::BitPCP pcp2_2(std::move(std::vector<pcp::BitDomain>{0, 1}));
        pcp2_1.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        pcp2_2.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        std::vector<std::pair<pcp::BitPCP, analyzer::Satisfiability>> samples = {
            {pcp2_1, true},
            {pcp2_2, false},
        };
        analyzer::PCPAnalyzer analyzer(samples, 10);
        double soundness = analyzer.getSoundness();
        double completeness = analyzer.getCompleteness();
        double gap = analyzer.getGap();

        // Check if values make sense
        assert(completeness == 1.0);  // Completeness should always be 1
        assert(soundness == 0);     // No unsatisfiable samples are accepted
        assert(abs(gap - (completeness - soundness)) < 1e-5); // correct gap calculation
    },
    []() -> void {
        // Test case 3: Complex PCP with multiple constraints
        pcp::BitPCP pcp3(std::vector<pcp::BitDomain>{0, 1, 1, 0});
        pcp3.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        pcp3.add_constraint(1, 2, constraint::BitConstraint::EQUAL);
        pcp3.add_constraint(2, 3, constraint::BitConstraint::NOTEQUAL);
        std::vector<std::pair<pcp::BitPCP, analyzer::Satisfiability>> samples = {
            {pcp3, false},
        };
        analyzer::PCPAnalyzer analyzer(samples, 10000);
        double soundness = analyzer.getSoundness();
        double completeness = analyzer.getCompleteness();
        double gap = analyzer.getGap();

        // Check if values make sense
        assert(completeness == 1.0); // Completeness should always be 1
        assert(abs(soundness - 0.6666) < 1e-2); // 1 out of 3 constraints violated
        assert(abs(gap - (completeness - soundness)) < 1e-5); // Correct gap calculation
    },
    []() -> void {
        // Test case 4: Complex PCP with more constraints
        pcp::BitPCP pcp4(std::vector<pcp::BitDomain>{0, 1, 1, 0, 1});
        pcp4.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
        pcp4.add_constraint(1, 2, constraint::BitConstraint::EQUAL);
        pcp4.add_constraint(2, 3, constraint::BitConstraint::NOTEQUAL);
        pcp4.add_constraint(3, 4, constraint::BitConstraint::EQUAL);
        pcp4.add_constraint(0, 4, constraint::BitConstraint::NOTEQUAL);
        std::vector<std::pair<pcp::BitPCP, analyzer::Satisfiability>> samples = {
            {pcp4, false},
        };
        analyzer::PCPAnalyzer analyzer(samples, 10000);
        double soundness = analyzer.getSoundness();
        double completeness = analyzer.getCompleteness();
        double gap = analyzer.getGap();

        // Check if values make sense
        assert(completeness == 1.0); // Completeness should always be 1
        assert(abs(soundness - 0.6) < 1e-2); // 2 out of 5 constraints violated
        assert(abs(gap - (completeness - soundness)) < 1e-5); // Correct gap calculation
    },
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}