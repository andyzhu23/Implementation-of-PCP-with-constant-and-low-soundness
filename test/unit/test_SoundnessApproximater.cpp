#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "analyzer/SoundnessApproximater.hpp"
#include "pcp/BinaryCSP.hpp"
#include "pcp/BinaryDomain.hpp"
#include "constraint/BinaryConstraint.hpp"
#include <cmath>

std::vector<std::function<void()>> test_cases = {
    []() {
        using namespace pcp;
        using namespace three_csp;
        // Single variable, trivially satisfiable
        std::vector<BinaryDomain> vars1 = { BinaryDomain(false, false, false, Constraint::ANY) };
        BinaryCSP p1(std::move(vars1));
        double res1 = analyzer::approximate_soundness(p1);
        assert(std::fabs(res1 - 1.0) < 1e-9);
    },
    []() {
        using namespace pcp;
        using namespace three_csp;
        using namespace constraint;
        // Two variables with EQUAL constraint and ENCODED_BINARY domain -> fully satisfiable
        std::vector<BinaryDomain> vars2 = {
            BinaryDomain(false, false, false, Constraint::ENCODED_BINARY),
            BinaryDomain(false, false, false, Constraint::ENCODED_BINARY)
        };
        BinaryCSP p2(std::move(vars2));
        p2.add_constraint(0, 1, BinaryConstraint::EQUAL);
        double res2 = analyzer::approximate_soundness(p2);
        assert(std::fabs(res2 - 1.0) < 1e-9);
    },
    []() {
        using namespace pcp;
        using namespace three_csp;
        using namespace constraint;
        // Triangle with contradictory constraints: 0-1 EQUAL, 1-2 EQUAL, 2-0 NOTEQUAL
        // For any domain where multiple values exist this forces at most 2 satisfiable
        std::vector<BinaryDomain> vars3 = {
            BinaryDomain(false, false, false, Constraint::ANY),
            BinaryDomain(false, false, false, Constraint::ANY),
            BinaryDomain(false, false, false, Constraint::ANY)
        };
        BinaryCSP p3(std::move(vars3));
        p3.add_constraint(0, 1, BinaryConstraint::EQUAL);
        p3.add_constraint(1, 2, BinaryConstraint::EQUAL);
        p3.add_constraint(2, 0, BinaryConstraint::NOTEQUAL);
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