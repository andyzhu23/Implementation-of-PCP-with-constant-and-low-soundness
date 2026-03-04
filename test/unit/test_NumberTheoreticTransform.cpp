#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <cmath>

#include "finite_field/NumberTheoreticTransform.hpp"
#include "finite_field/FiniteFieldElement.hpp"
#include "finite_field/constant.hpp"

using finite_field::FiniteFieldElement;
using finite_field::ntt;
using finite_field::intt;
using finite_field::convolution;

// Helper function to check if two vectors are approximately equal
bool vectors_equal(std::vector<FiniteFieldElement> a, std::vector<FiniteFieldElement> b) {

    while (!a.empty() && a.back().getValue() == 0) a.pop_back(); // Remove trailing zeros
    while (!b.empty() && b.back().getValue() == 0) b.pop_back(); // Remove trailing zeros

    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].getValue() != b[i].getValue()) return false;
    }
    return true;
}

std::vector<std::function<void()>> test_cases = {
    // Test 1: NTT-INTT identity for a single element vector
    []() -> void {
        std::vector<FiniteFieldElement> input = {FiniteFieldElement(42)};
        auto ntt_result = ntt(input);
        auto restored = intt(ntt_result);
        assert(vectors_equal(restored, {FiniteFieldElement(42)}) && "NTT-INTT should be identity for single element");
    },

    // Test 2: INTT-NTT identity for a single element vector
    []() -> void {
        std::vector<FiniteFieldElement> input = {FiniteFieldElement(123)};
        auto intt_result = intt(input);
        auto restored = ntt(intt_result);
        assert(vectors_equal(restored, {FiniteFieldElement(123)}) && "INTT-NTT should be identity for single element");
    },

    // Test 3: NTT-INTT identity for a power-of-2 sized vector
    []() -> void {
        std::vector<FiniteFieldElement> input = {
            FiniteFieldElement(1), FiniteFieldElement(2), 
            FiniteFieldElement(3), FiniteFieldElement(4)
        };
        auto ntt_result = ntt(input);
        auto restored = intt(ntt_result);
        
        assert(vectors_equal(restored, input) && "NTT-INTT should be identity for power-of-2 vector");
    },

    // Test 4: INTT-NTT identity for a power-of-2 sized vector  
    []() -> void {
        std::vector<FiniteFieldElement> input = {
            FiniteFieldElement(7), FiniteFieldElement(14), 
            FiniteFieldElement(21), FiniteFieldElement(28)
        };
        auto intt_result = intt(input);
        auto restored = ntt(intt_result);
        assert(vectors_equal(restored, input) && "INTT-NTT should be identity for power-of-2 vector");
    },

    // Test 5: NTT-INTT identity for larger vector (8 elements)
    []() -> void {
        std::vector<FiniteFieldElement> input;
        for (int i = 1; i <= 8; ++i) {
            input.push_back(FiniteFieldElement(i * 10));
        }
        auto ntt_result = ntt(input);
        auto restored = intt(ntt_result);
        assert(vectors_equal(restored, input) && "NTT-INTT should be identity for 8-element vector");
    },

    // Test 6: INTT-NTT identity for larger vector (8 elements)
    []() -> void {
        std::vector<FiniteFieldElement> input;
        for (int i = 1; i <= 8; ++i) {
            input.push_back(FiniteFieldElement(i * 100));
        }
        auto intt_result = intt(input);
        auto restored = ntt(intt_result);
        assert(vectors_equal(restored, input) && "INTT-NTT should be identity for 8-element vector");
    },

    // Test 7: Convolution identity - convolution of [1,0,0,...] with x should be x
    []() -> void {
        std::vector<FiniteFieldElement> identity = {FiniteFieldElement(1), FiniteFieldElement(0)};
        std::vector<FiniteFieldElement> x = {FiniteFieldElement(5), FiniteFieldElement(3)};
        auto result = convolution(identity, x);
        // Convolution result should be padded to power of 2, so we check first elements
        assert(result.size() >= 2 && "Convolution result should have at least 2 elements");
        assert(result[0].getValue() == 5 && result[1].getValue() == 3 && 
               "Convolution with identity should preserve the vector");
    },

    // Test 8: Convolution commutativity - conv(a,b) = conv(b,a)
    []() -> void {
        std::vector<FiniteFieldElement> a = {FiniteFieldElement(1), FiniteFieldElement(2)};
        std::vector<FiniteFieldElement> b = {FiniteFieldElement(3), FiniteFieldElement(4)};
        auto conv_ab = convolution(a, b);
        auto conv_ba = convolution(b, a);
        assert(vectors_equal(conv_ab, conv_ba) && "Convolution should be commutative");
    },

    // Test 9: Convolution with zero vector
    []() -> void {
        std::vector<FiniteFieldElement> a = {FiniteFieldElement(5), FiniteFieldElement(7)};
        std::vector<FiniteFieldElement> zero = {FiniteFieldElement(0), FiniteFieldElement(0)};
        auto result = convolution(a, zero);
        // Result should be all zeros (up to padding)
        bool all_zero = true;
        for (const auto& elem : result) {
            if (elem.getValue() != 0) {
                all_zero = false;
                break;
            }
        }
        assert(all_zero && "Convolution with zero vector should give zero");
    },

    // Test 10: Manual convolution verification for simple case
    []() -> void {
        // (1 + 2x) * (3 + 4x) = 3 + 4x + 6x + 8x² = 3 + 10x + 8x²
        std::vector<FiniteFieldElement> poly1 = {FiniteFieldElement(1), FiniteFieldElement(2)};
        std::vector<FiniteFieldElement> poly2 = {FiniteFieldElement(3), FiniteFieldElement(4)};
        auto result = convolution(poly1, poly2);
        
        assert(result.size() >= 3 && "Convolution result should have at least 3 coefficients");
        assert(result[0].getValue() == 3 && "Constant term should be 3");
        assert(result[1].getValue() == 10 && "x coefficient should be 10");
        assert(result[2].getValue() == 8 && "x² coefficient should be 8");
    },

    // Test 11: NTT-INTT identity with non-power-of-2 input (gets padded)
    []() -> void {
        std::vector<FiniteFieldElement> input = {
            FiniteFieldElement(11), FiniteFieldElement(22), FiniteFieldElement(33)
        };
        auto original_size = input.size();
        auto ntt_result = ntt(input);
        auto restored = intt(ntt_result);
        
        // Check that the first original_size elements match
        assert(restored.size() >= original_size && "Restored vector should be at least as large as input");
        for (size_t i = 0; i < original_size; ++i) {
            assert(restored[i].getValue() == input[i].getValue() && 
                   "NTT-INTT should preserve original elements after padding");
        }
    },

    // Test 12: Large vector NTT-INTT identity (16 elements)
    []() -> void {
        std::vector<FiniteFieldElement> input;
        for (int i = 1; i <= 16; ++i) {
            input.push_back(FiniteFieldElement(i * 7)); // Use 7 to avoid simple patterns
        }
        auto ntt_result = ntt(input);
        auto restored = intt(ntt_result);
        assert(vectors_equal(restored, input) && "NTT-INTT should be identity for 16-element vector");
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
