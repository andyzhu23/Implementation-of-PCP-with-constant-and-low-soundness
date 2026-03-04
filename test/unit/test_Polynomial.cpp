#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "finite_field/Polynomial.hpp"
#include "finite_field/Monomial.hpp"
#include "finite_field/FiniteFieldElement.hpp"
#include "finite_field/constant.hpp"

using finite_field::FiniteFieldElement;
using finite_field::Monomial;
using finite_field::Polynomial;

std::vector<std::function<void()>> test_cases = {
    // Test 1: Default constructor produces empty polynomial that evaluates to 0
    []() -> void {
        Polynomial p;
        std::vector<FiniteFieldElement> vars = {FiniteFieldElement(3)};
        assert(p.evaluate(vars).getValue() == 0 && "Empty polynomial should evaluate to 0");
    },

    // Test 2: Single constant monomial (no variables)
    []() -> void {
        // p = 5  (coefficient=5, no variable exponents)
        Monomial m(FiniteFieldElement(5), std::vector<size_t>{});
        Polynomial p(std::vector<Monomial>{m});
        assert(p.evaluate({}).getValue() == 5 && "Constant polynomial should evaluate to its coefficient");
    },

    // Test 3: Single univariate monomial 3*x^2 at x=2 => 3*4=12
    []() -> void {
        Monomial m(FiniteFieldElement(3), std::vector<size_t>{2});
        Polynomial p(std::vector<Monomial>{m});
        // 3 * 2^2 = 12
        assert(p.evaluate({FiniteFieldElement(2)}).getValue() == 12
            && "3*x^2 at x=2 should be 12");
    },

    // Test 4: Lvalue vector constructor sets terms correctly
    []() -> void {
        std::vector<Monomial> terms = {
            Monomial(FiniteFieldElement(1), std::vector<size_t>{1}),  // 1*x
            Monomial(FiniteFieldElement(2), std::vector<size_t>{0})   // 2*x^0 = 2
        };
        Polynomial p(terms);
        // p(3) = 3 + 2 = 5
        assert(p.evaluate({FiniteFieldElement(3)}).getValue() == 5
            && "p(3) = 1*3 + 2 = 5");
    },

    // Test 5: Rvalue (move) vector constructor
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(4), std::vector<size_t>{1})   // 4*x
        });
        // p(2) = 4*2 = 8
        assert(p.evaluate({FiniteFieldElement(2)}).getValue() == 8
            && "4*x at x=2 should be 8");
    },

    // Test 6: addTerm (lvalue) adds a term and it contributes to evaluation
    []() -> void {
        Polynomial p;
        Monomial m(FiniteFieldElement(3), std::vector<size_t>{1});  // 3*x
        p.addTerm(m);
        // p(1) = 3
        assert(p.evaluate({FiniteFieldElement(1)}).getValue() == 3
            && "addTerm (lvalue): p(1) should be 3");
    },

    // Test 7: addTerm (rvalue) adds a term and it contributes to evaluation
    []() -> void {
        Polynomial p;
        p.addTerm(Monomial(FiniteFieldElement(5), std::vector<size_t>{1}));  // constant 5
        assert(p.evaluate({1}).getValue() == 5
            && "addTerm (rvalue): constant term should evaluate to 5");
    },

    // Test 8: Multiple terms sum correctly - p(x) = x + x^2 at x=3 => 3+9=12
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(1), std::vector<size_t>{1}),  // x
            Monomial(FiniteFieldElement(1), std::vector<size_t>{2})   // x^2
        });
        // 3 + 9 = 12
        assert(p.evaluate({FiniteFieldElement(3)}).getValue() == 12
            && "x + x^2 at x=3 should be 12");
    },

    // Test 9: Multivariate monomial 2*x*y at x=3, y=2 => 2*3*2=12
    []() -> void {
        Monomial m(FiniteFieldElement(2), std::vector<size_t>{1, 1});  // 2*x^1*y^1
        Polynomial p(std::vector<Monomial>{m});
        assert(p.evaluate({FiniteFieldElement(3), FiniteFieldElement(2)}).getValue() == 12
            && "2*x*y at x=3, y=2 should be 12");
    },

    // Test 10: Multivariate polynomial p(x,y) = x^2 + y at (2,3) => 4+3=7
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(1), std::vector<size_t>{2, 0}),  // x^2
            Monomial(FiniteFieldElement(1), std::vector<size_t>{0, 1})   // y
        });
        assert(p.evaluate({FiniteFieldElement(2), FiniteFieldElement(3)}).getValue() == 7
            && "x^2 + y at (2,3) should be 7");
    },

    // Test 11: Zero coefficient monomial contributes nothing
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(0), std::vector<size_t>{3}),  // 0*x^3
            Monomial(FiniteFieldElement(6), std::vector<size_t>{0})   // 6
        });
        assert(p.evaluate({FiniteFieldElement(5)}).getValue() == 6
            && "Zero-coefficient term should not affect the result");
    },

    // Test 12: Evaluate at x=0 with x^n terms (all vanish) leaves only constant
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(7), std::vector<size_t>{0}),  // constant 7
            Monomial(FiniteFieldElement(3), std::vector<size_t>{1}),  // 3x
            Monomial(FiniteFieldElement(2), std::vector<size_t>{2})   // 2x^2
        });
        assert(p.evaluate({FiniteFieldElement(0)}).getValue() == 7
            && "At x=0 only the constant term survives");
    },

    // Test 13: Modular wrap-around in result - use large coefficients
    []() -> void {
        // Use coefficients that will cause wrapping: 998244353 wraps to 0
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(998244353), std::vector<size_t>{0}),  // constant term that wraps to 0
            Monomial(FiniteFieldElement(1), std::vector<size_t>{0})   // constant 1
        });
        assert(p.evaluate({FiniteFieldElement(0)}).getValue() == 1
            && "998244353 + 1 should wrap to 1");
    },

    // Test 14: evaluate throws for mismatched variable count
    []() -> void {
        Monomial m(FiniteFieldElement(1), std::vector<size_t>{1, 1});  // needs 2 vars
        Polynomial p(std::vector<Monomial>{m});
        bool threw = false;
        try {
            p.evaluate({FiniteFieldElement(1)});  // only 1 var supplied
        } catch (const std::invalid_argument &) {
            threw = true;
        }
        assert(threw && "evaluate should throw for mismatched variable count");
    },

    // Test 15: addTerm then evaluate - incremental build of p(x)=1+2x+3x^2
    []() -> void {
        Polynomial p;
        p.addTerm(Monomial(FiniteFieldElement(1), std::vector<size_t>{0}));  // 1
        p.addTerm(Monomial(FiniteFieldElement(2), std::vector<size_t>{1}));  // 2x
        p.addTerm(Monomial(FiniteFieldElement(3), std::vector<size_t>{2}));  // 3x^2
        // p(2) = 1 + 4 + 12 = 17
        assert(p.evaluate({FiniteFieldElement(2)}).getValue() == 17
            && "1 + 2*2 + 3*4 = 17");
        // p(1) = 1 + 2 + 3 = 6
        assert(p.evaluate({FiniteFieldElement(1)}).getValue() == 6
            && "p(1) = 1+2+3 = 6");
    },

    // Test 16: Three-variable sum p(x,y,z) = x + y + z at (1,2,3) => 6
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(1), std::vector<size_t>{1, 0, 0}),  // x
            Monomial(FiniteFieldElement(1), std::vector<size_t>{0, 1, 0}),  // y
            Monomial(FiniteFieldElement(1), std::vector<size_t>{0, 0, 1})   // z
        });
        // 1+2+3 = 6
        assert(p.evaluate({FiniteFieldElement(1), FiniteFieldElement(2), FiniteFieldElement(3)}).getValue() == 6
            && "x+y+z at (1,2,3) should be 6");
        // (3+5+6) = 14
        assert(p.evaluate({FiniteFieldElement(3), FiniteFieldElement(5), FiniteFieldElement(6)}).getValue() == 14
            && "x+y+z at (3,5,6) should be 14");
    },

    // Test 17: Mixed-degree bivariate monomial x^2*y at (2,3) => 4*3=12; at (3,2) => 9*2=18
    []() -> void {
        Monomial m(FiniteFieldElement(1), std::vector<size_t>{2, 1});  // x^2 * y
        Polynomial p(std::vector<Monomial>{m});
        // 2^2 * 3 = 4*3 = 12
        assert(p.evaluate({FiniteFieldElement(2), FiniteFieldElement(3)}).getValue() == 12
            && "x^2*y at (2,3) should be 12");
        // 3^2 * 2 = 9*2 = 18
        assert(p.evaluate({FiniteFieldElement(3), FiniteFieldElement(2)}).getValue() == 18
            && "x^2*y at (3,2) should be 18");
    },

    // Test 18: Three-variable monomial x*y*z at various points
    []() -> void {
        Monomial m(FiniteFieldElement(1), std::vector<size_t>{1, 1, 1});  // x*y*z
        Polynomial p(std::vector<Monomial>{m});
        // 1*2*3 = 6
        assert(p.evaluate({FiniteFieldElement(1), FiniteFieldElement(2), FiniteFieldElement(3)}).getValue() == 6
            && "x*y*z at (1,2,3) should be 6");
        // 2*3*4 = 24
        assert(p.evaluate({FiniteFieldElement(2), FiniteFieldElement(3), FiniteFieldElement(4)}).getValue() == 24
            && "x*y*z at (2,3,4) should be 24");
    },

    // Test 19: Three-variable polynomial xy + yz + xz at (1,2,3) => 2+6+3=11
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(1), std::vector<size_t>{1, 1, 0}),  // x*y
            Monomial(FiniteFieldElement(1), std::vector<size_t>{0, 1, 1}),  // y*z
            Monomial(FiniteFieldElement(1), std::vector<size_t>{1, 0, 1})   // x*z
        });
        // xy=2, yz=6, xz=3 => 11
        assert(p.evaluate({FiniteFieldElement(1), FiniteFieldElement(2), FiniteFieldElement(3)}).getValue() == 11
            && "xy+yz+xz at (1,2,3) should be 11");
    },

    // Test 20: Bivariate polynomial with coefficient: 2*x*y^2 + 3*x^2*y at (2,3)
    []() -> void {
        Polynomial p(std::vector<Monomial>{
            Monomial(FiniteFieldElement(2), std::vector<size_t>{1, 2}),  // 2*x*y^2
            Monomial(FiniteFieldElement(3), std::vector<size_t>{2, 1})   // 3*x^2*y
        });
        // 2*x*y^2 at (2,3): 2*2*9 = 36
        // 3*x^2*y at (2,3): 3*4*3 = 36
        // total: 36+36 = 72
        assert(p.evaluate({FiniteFieldElement(2), FiniteFieldElement(3)}).getValue() == 72
            && "2*x*y^2 + 3*x^2*y at (2,3) should be 72");
        // at (1,1): 2*1*1 + 3*1*1 = 5
        assert(p.evaluate({FiniteFieldElement(1), FiniteFieldElement(1)}).getValue() == 5
            && "2*x*y^2 + 3*x^2*y at (1,1) should be 5");
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
