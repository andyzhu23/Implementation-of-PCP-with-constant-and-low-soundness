#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "finite_field/UnivariatePolynomial.hpp"
#include "finite_field/FiniteFieldElement.hpp"
#include "finite_field/constant.hpp"

using finite_field::FiniteFieldElement;
using finite_field::UnivariatePolynomial;

std::vector<std::function<void()>> test_cases = {
    // Test 1: Default constructor produces degree-0 zero polynomial
    []() -> void {
        UnivariatePolynomial p;
        assert(p.getDegree() == 0 && "Default constructor should produce degree-0 polynomial");
        assert(p[0].getValue() == 0 && "Default constructor coefficient should be 0");
    },

    // Test 2: Degree constructor produces zero polynomial of correct degree
    []() -> void {
        UnivariatePolynomial p(3);
        assert(p.getDegree() == 3 && "Degree constructor should set degree correctly");
        for (size_t i = 0; i <= 3; ++i) {
            assert(p[i].getValue() == 0 && "Degree constructor should initialise all coefficients to 0");
        }
    },

    // Test 3: Coefficient vector constructor (lvalue) sets degree and coefficients correctly
    []() -> void {
        std::vector<FiniteFieldElement> coeffs = {FiniteFieldElement(1), FiniteFieldElement(2), FiniteFieldElement(3)};
        UnivariatePolynomial p(coeffs);
        assert(p.getDegree() == 2 && "Coefficient vector constructor should set degree to size-1");
        assert(p[0].getValue() == 1 && "Coefficient [0] should be 1");
        assert(p[1].getValue() == 2 && "Coefficient [1] should be 2");
        assert(p[2].getValue() == 3 && "Coefficient [2] should be 3");
    },

    // Test 4: Coefficient vector constructor (rvalue/move) sets degree and coefficients correctly
    []() -> void {
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{FiniteFieldElement(4), FiniteFieldElement(5)});
        assert(p.getDegree() == 1 && "Move constructor should set degree to size-1");
        assert(p[0].getValue() == 4 && "Coefficient [0] should be 4");
        assert(p[1].getValue() == 5 && "Coefficient [1] should be 5");
    },

    // Test 5: operator[] returns 0 for out-of-range index
    []() -> void {
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{FiniteFieldElement(1), FiniteFieldElement(2)});
        assert(p[5].getValue() == 0 && "Out-of-range index should return 0");
        assert(p[100].getValue() == 0 && "Far out-of-range index should return 0");
    },

    // Test 6: evaluate at x=0 returns constant term
    []() -> void {
        // p(x) = 5 + 3x + 2x^2
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{
            FiniteFieldElement(5), FiniteFieldElement(3), FiniteFieldElement(2)
        });
        assert(p.evaluate(FiniteFieldElement(0)).getValue() == 5 && "p(0) should equal the constant term");
    },

    // Test 7: evaluate at x=1 returns sum of all coefficients (mod field size)
    []() -> void {
        // p(x) = 1 + 2x + 3x^2;  p(1) = (1+2+3) mod 8 = 6
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{
            FiniteFieldElement(1), FiniteFieldElement(2), FiniteFieldElement(3)
        });
        int expected = (1 + 2 + 3) % finite_field::FINITE_FIELD_SIZE;
        assert(p.evaluate(FiniteFieldElement(1)).getValue() == expected && "p(1) should be sum of coefficients mod field size");
    },

    // Test 8: evaluate constant polynomial at several points
    []() -> void {
        // p(x) = 7  (constant)
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{FiniteFieldElement(7)});
        for (int x = 0; x < finite_field::FINITE_FIELD_SIZE; ++x) {
            assert(p.evaluate(FiniteFieldElement(x)).getValue() == 7 && "Constant polynomial should evaluate to constant everywhere");
        }
    },

    // Test 9: evaluate linear polynomial p(x) = 2 + 3x at several points
    []() -> void {
        // p(0)=2, p(1)=5, p(2)=(2+6)%8=0, p(3)=(2+9)%8=3
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{FiniteFieldElement(2), FiniteFieldElement(3)});
        assert(p.evaluate(FiniteFieldElement(0)).getValue() == 2 && "p(0) should be 2");
        assert(p.evaluate(FiniteFieldElement(1)).getValue() == 5 && "p(1) should be 5");
        assert(p.evaluate(FiniteFieldElement(2)).getValue() == 0 && "p(2) should be 0 (mod 8)");
        assert(p.evaluate(FiniteFieldElement(3)).getValue() == 3 && "p(3) should be 3 (mod 8)");
    },

    // Test 10: evaluate quadratic polynomial p(x) = 1 + 2x + 3x^2
    []() -> void {
        // p(0)=1, p(1)=6, p(2)=(1+4+12)%8=1, p(3)=(1+6+27)%8=2
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{
            FiniteFieldElement(1), FiniteFieldElement(2), FiniteFieldElement(3)
        });
        assert(p.evaluate(FiniteFieldElement(0)).getValue() == 1  && "p(0) should be 1");
        assert(p.evaluate(FiniteFieldElement(1)).getValue() == 6  && "p(1) should be 6");
        assert(p.evaluate(FiniteFieldElement(2)).getValue() == 1  && "p(2) should be 1 (mod 8)");
        assert(p.evaluate(FiniteFieldElement(3)).getValue() == 2  && "p(3) should be 2 (mod 8)");
    },

    // Test 11: evaluate zero polynomial returns 0 at all points
    []() -> void {
        UnivariatePolynomial p(3); // degree 3, all zero coefficients
        for (int x = 0; x < finite_field::FINITE_FIELD_SIZE; ++x) {
            assert(p.evaluate(FiniteFieldElement(x)).getValue() == 0 && "Zero polynomial should evaluate to 0 everywhere");
        }
    },

    // Test 12: getDegree for single-element coefficient vector is 0
    []() -> void {
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{FiniteFieldElement(6)});
        assert(p.getDegree() == 0 && "Single-coefficient polynomial should have degree 0");
    },

    // Test 13: evaluate with field-size wrapping in coefficients
    []() -> void {
        // FiniteFieldElement wraps on construction: value 9 -> 1 (9 % 8)
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{FiniteFieldElement(9), FiniteFieldElement(0)});
        assert(p[0].getValue() == 1 && "Coefficient 9 should wrap to 1 mod 8");
        assert(p.evaluate(FiniteFieldElement(2)).getValue() == 1 && "p(2) = 1 + 0*2 = 1");
    },

    // Test 14: evaluate high-degree polynomial p(x) = x^4 at x=2
    []() -> void {
        // coeffs: [0, 0, 0, 0, 1]  => p(x) = x^4
        // p(2) = 16 % 8 = 0
        UnivariatePolynomial p(std::vector<FiniteFieldElement>{
            FiniteFieldElement(0), FiniteFieldElement(0), FiniteFieldElement(0),
            FiniteFieldElement(0), FiniteFieldElement(1)
        });
        assert(p.getDegree() == 4 && "Degree should be 4");
        assert(p.evaluate(FiniteFieldElement(2)).getValue() == 0 && "2^4=16, 16%8=0");
        assert(p.evaluate(FiniteFieldElement(3)).getValue() == 1 && "3^4=81, 81%8=1");
    },

    // Test 15: operator[] returns the correct coefficient at every valid index
    []() -> void {
        std::vector<FiniteFieldElement> coeffs;
        for (int i = 0; i < (int)finite_field::FINITE_FIELD_SIZE; ++i) {
            coeffs.push_back(FiniteFieldElement(i));
        }
        UnivariatePolynomial p(coeffs);
        assert(p.getDegree() == finite_field::FINITE_FIELD_SIZE - 1 && "Degree should be FINITE_FIELD_SIZE - 1");
        for (size_t i = 0; i < finite_field::FINITE_FIELD_SIZE; ++i) {
            assert(p[i].getValue() == (int)i && "operator[] should return correct coefficient at each index");
        }
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
