#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "analyzer/SoundnessApproximater.hpp"
#include "pcp/BitPCP.hpp"
#include "pcp/BitDomain.hpp"
#include "constraint/BitConstraint.hpp"

std::vector<std::function<void()>> test_cases = {
    []() {
        using namespace pcp;
        using namespace three_csp;
        // Single variable, trivially satisfiable
        std::vector<BitDomain> vars1 = { BitDomain(false, false, false, Constraint::ANY) };
        BitPCP p1(std::move(vars1));
        double res1 = analyzer::approximate_soundness(p1);
        assert(std::fabs(res1 - 1.0) < 1e-9);
    },
    []() {
        using namespace pcp;
        using namespace three_csp;
        using namespace constraint;
        // Two variables with EQUAL constraint and ENCODED_BINARY domain -> fully satisfiable
        std::vector<BitDomain> vars2 = {
            BitDomain(false, false, false, Constraint::ENCODED_BINARY),
            BitDomain(false, false, false, Constraint::ENCODED_BINARY)
        };
        BitPCP p2(std::move(vars2));
        p2.add_constraint(0, 1, BitConstraint::EQUAL);
        double res2 = analyzer::approximate_soundness(p2);
        assert(std::fabs(res2 - 1.0) < 1e-9);
    },
    []() {
        using namespace pcp;
        using namespace three_csp;
        using namespace constraint;
        // Triangle with contradictory constraints: 0-1 EQUAL, 1-2 EQUAL, 2-0 NOTEQUAL
        // For any domain where multiple values exist this forces at most 2 satisfiable
        std::vector<BitDomain> vars3 = {
            BitDomain(false, false, false, Constraint::ANY),
            BitDomain(false, false, false, Constraint::ANY),
            BitDomain(false, false, false, Constraint::ANY)
        };
        BitPCP p3(std::move(vars3));
        p3.add_constraint(0, 1, BitConstraint::EQUAL);
        p3.add_constraint(1, 2, BitConstraint::EQUAL);
        p3.add_constraint(2, 0, BitConstraint::NOTEQUAL);
        double res3 = analyzer::approximate_soundness(p3);
        assert(std::fabs(res3 - (2.0/3.0)) < 1e-9);
    }
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}