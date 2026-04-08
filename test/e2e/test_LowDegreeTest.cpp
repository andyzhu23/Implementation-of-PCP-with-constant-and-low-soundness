#include <cassert>
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

#include "finite_field/Polynomial.hpp"
#include "pcpp/ReedMullerPCPP/ReedMuller.hpp"
#include "pcpp/ReedMullerPCPP/LowDegreeTest.hpp"

std::vector<std::function<void()>> test_cases = {
    []() -> void {
        // p(x, y, z) = x + y + z
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {1, 0, 0}), // x
            finite_field::Monomial(1, {0, 1, 0}), // y
            finite_field::Monomial(1, {0, 0, 1})  // z
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(3, oracle), 1);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify the univariate polynomial correctly represents the evaluation of the Reed-Muller code on the random line");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(3, oracle), 0);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    },
    []() -> void {
        // p(x, y, z) = x^2 + y^2 + z^2
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {2, 0, 0}), // x^2
            finite_field::Monomial(1, {0, 2, 0}), // y^2
            finite_field::Monomial(1, {0, 0, 2})  // z^2
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(3, oracle), 2);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify the univariate polynomial correctly represents the evaluation of the Reed-Muller code on the random line");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(3, oracle), 1);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    }, 
    []() -> void {
        // p(x, y, z, w) = 3*x^3*y^2 + 9*y*z^5 + z*x^4 + 6*x^2*y^2*z^2 + 2*w^5*x + 5
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(3, {3, 2, 0, 0}), // 3*x^3*y^2
            finite_field::Monomial(9, {0, 1, 5, 0}), // 9*y*z^5
            finite_field::Monomial(1, {4, 0, 1, 0}), // z*x^4
            finite_field::Monomial(6, {2, 2, 2, 0}), // 6*x^2*y^2*z^2
            finite_field::Monomial(2, {1, 0, 0, 5}), // 2*w^5*x
            finite_field::Monomial(5, {0, 0, 0, 0})  // constant term 5
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(4, oracle), 6);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify the univariate polynomial correctly represents the evaluation of the Reed-Muller code on the random line");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(4, oracle), 5);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    }, 
    []() -> void {
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [](const std::vector<finite_field::FiniteFieldElement>& input) {
            return finite_field::get_random_element();
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(100, oracle), 1000);
        // Since the oracle is random, the univariate polynomial will not match the oracle's evaluation on the random line, so verification should fail with high probability
        assert(!ldt.verifyPolynomial() && "LowDegreeTest with random oracle should fail verification with high probability");
    },
    []() -> void {
        // High degree polynomial with 10 variables, total degree 100
        // p(x0, x1, ..., x9) = x0^10 * x1^10 * x2^10 * x3^10 * x4^10 * x5^10 * x6^10 * x7^10 * x8^10 * x9^10
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {10, 10, 10, 10, 10, 10, 10, 10, 10, 10}) // total degree = 100
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(10, oracle), 100);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify polynomial with 10 variables and degree 100");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(10, oracle), 99);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    },
    []() -> void {
        // Complex polynomial with 8 variables, various terms with total degree up to 100
        // p = 7*x0^50*x1^50 + 3*x2^30*x3^30*x4^40 + 11*x5^25*x6^25*x7^25*x0^25 + 13*x1^100 + 2
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(7, {50, 50, 0, 0, 0, 0, 0, 0}),   // 7*x0^50*x1^50 (degree 100)
            finite_field::Monomial(3, {0, 0, 30, 30, 40, 0, 0, 0}),  // 3*x2^30*x3^30*x4^40 (degree 100)
            finite_field::Monomial(11, {25, 0, 0, 0, 0, 25, 25, 25}),// 11*x5^25*x6^25*x7^25*x0^25 (degree 100)
            finite_field::Monomial(13, {0, 100, 0, 0, 0, 0, 0, 0}),  // 13*x1^100 (degree 100)
            finite_field::Monomial(2, {0, 0, 0, 0, 0, 0, 0, 0})      // constant term 2 (degree 0)
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(8, oracle), 100);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify complex polynomial with 8 variables");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(8, oracle), 99);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    },
    []() -> void {
        // 15 variables with mixed degrees summing to 100
        // p = 5*x0^10*x1^10*x2^10*x3^10*x4^10*x5^10*x6^10*x7^10*x8^5*x9^5*x10^5*x11^3*x12^1*x13^1 + 
        //     9*x14^100 + x0^20*x14^80
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(5, {10, 10, 10, 10, 10, 10, 10, 10, 5, 5, 5, 3, 1, 1, 0}), // degree = 100
            finite_field::Monomial(9, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100}),       // 9*x14^100
            finite_field::Monomial(1, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 80})        // x0^20*x14^80
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(15, oracle), 100);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify polynomial with 15 variables and degree 100");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(15, oracle), 99);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    },
    []() -> void {
        // 20 variables with degree 100, many terms
        // p = sum of several monomials each with total degree 100
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}),  // each var^5, degree 100
            finite_field::Monomial(2, {100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}), // x0^100
            finite_field::Monomial(3, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100}), // x19^100
            finite_field::Monomial(4, {50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50}), // x0^50*x19^50
            finite_field::Monomial(17, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})   // constant 17
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(20, oracle), 100);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify polynomial with 20 variables and degree 100");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(20, oracle), 99);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    },
    []() -> void {
        // Lower degree (50) with 12 variables and many terms to test variety
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(7, {10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0}),  // degree 50
            finite_field::Monomial(11, {0, 0, 0, 0, 0, 10, 10, 10, 10, 10, 0, 0}), // degree 50
            finite_field::Monomial(13, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 25}),    // degree 50
            finite_field::Monomial(19, {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0}),      // degree 50
            finite_field::Monomial(23, {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5}),      // degree 50
            finite_field::Monomial(29, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50}),     // x11^50
            finite_field::Monomial(31, {50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),     // x0^50
            finite_field::Monomial(1, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})        // constant 1
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };
        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(12, oracle), 50);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify polynomial with 12 variables and degree 50");

        pcpp::LowDegreeTest ldt_wrong_bound(pcpp::ReedMuller(12, oracle), 49);
        assert(!ldt_wrong_bound.verifyPolynomial() && "LowDegreeTest with wrong degree bound should fail verification");
    }, 
    []() -> void {
        // Honest prover with very high degree (< 65537) in one variable.
        // p(x) = 9*x^65000 + 4*x^32000 + 17
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(9, {65000}),
            finite_field::Monomial(4, {32000}),
            finite_field::Monomial(17, {0})
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };

        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(1, oracle), 65000);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify honest prover with degree 65000");
    },
    []() -> void {
        // Honest prover with very high total degree (< 65537) in three variables.
        // p(x,y,z) = 3*x^40000*y^25000 + 5*z^65000 + 11*x^15000*z^50000 + 1
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(3, {40000, 25000, 0}),
            finite_field::Monomial(5, {0, 0, 65000}),
            finite_field::Monomial(11, {15000, 0, 50000}),
            finite_field::Monomial(1, {0, 0, 0})
        });
        std::function<finite_field::FiniteFieldElement(const std::vector<finite_field::FiniteFieldElement>&)> oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            return p.evaluate(input);
        };

        pcpp::LowDegreeTest ldt(pcpp::ReedMuller(3, oracle), 65000);
        assert(ldt.verifyPolynomial() && "LowDegreeTest should verify honest prover with multivariate degree 65000");
    },
    []() -> void {
        // Dishonest prover case 1:
        // P(x,y,z) = x + y + z (low degree, honest target)
        // C(x): all coordinates are even
        // H(x): x^7 + y^7 + z^7 + 5 (high degree)
        // Prover returns P(x) if C(x), otherwise H(x).
        // This should be rejected with high probability.
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {1, 0, 0}),
            finite_field::Monomial(1, {0, 1, 0}),
            finite_field::Monomial(1, {0, 0, 1})
        });

        auto dishonest_oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            bool condition_all_even = std::all_of(input.begin(), input.end(), [](const auto& v) {
                return (v.getValue() % 2) == 0;
            });

            if (condition_all_even) {
                return p.evaluate(input); // P(x)
            }

            // H(x) = x^7 + y^7 + z^7 + 5
            return input[0].exp(7) + input[1].exp(7) + input[2].exp(7) + finite_field::FiniteFieldElement(5);
        };

        const int trials = 40;
        int accepted = 0;
        for (int i = 0; i < trials; ++i) {
            pcpp::LowDegreeTest ldt(pcpp::ReedMuller(3, dishonest_oracle), 1);
            if (ldt.verifyPolynomial()) {
                ++accepted;
            }
        }

        assert(accepted <= 10 && "Dishonest prover (piecewise P/H with parity condition) should be rejected in most trials");
    },
    []() -> void {
        // Dishonest prover case 2:
        // P(x,y,z) = x*y + z (degree 2)
        // C(x): x == z (diagonal condition)
        // H(x): P(x) + (x-z)^9
        // Prover behaves honestly on a structured subset and deviates elsewhere.
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {1, 1, 0}), // x*y
            finite_field::Monomial(1, {0, 0, 1})  // z
        });

        auto dishonest_oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            finite_field::FiniteFieldElement px = p.evaluate(input);
            bool on_diagonal = (input[0] == input[2]);
            if (on_diagonal) {
                return px; // P(x)
            }

            // H(x) = P(x) + (x-z)^9
            return px + (input[0] - input[2]).exp(9);
        };

        const int trials = 40;
        int accepted = 0;
        for (int i = 0; i < trials; ++i) {
            pcpp::LowDegreeTest ldt(pcpp::ReedMuller(3, dishonest_oracle), 2);
            if (ldt.verifyPolynomial()) {
                ++accepted;
            }
        }

        assert(accepted <= 12 && "Dishonest prover (honest on diagonal, high-degree elsewhere) should be rejected in most trials");
    },
    []() -> void {
        // Dishonest prover case 3:
        // P(x0,...,x4) = x0 + x1 + x2 + x3 + x4 (degree 1)
        // C(x): first coordinate is zero
        // H(x): random value
        // This models a prover that is honest on a tiny structured slice and adversarial otherwise.
        finite_field::Polynomial p(std::vector<finite_field::Monomial>{
            finite_field::Monomial(1, {1, 0, 0, 0, 0}),
            finite_field::Monomial(1, {0, 1, 0, 0, 0}),
            finite_field::Monomial(1, {0, 0, 1, 0, 0}),
            finite_field::Monomial(1, {0, 0, 0, 1, 0}),
            finite_field::Monomial(1, {0, 0, 0, 0, 1})
        });

        auto dishonest_oracle = [&p](const std::vector<finite_field::FiniteFieldElement>& input) {
            if (input[0] == 0) {
                return p.evaluate(input); // P(x)
            }
            return finite_field::get_random_element(); // H(x)
        };

        const int trials = 30;
        int accepted = 0;
        for (int i = 0; i < trials; ++i) {
            pcpp::LowDegreeTest ldt(pcpp::ReedMuller(5, dishonest_oracle), 1);
            if (ldt.verifyPolynomial()) {
                ++accepted;
            }
        }

        assert(accepted <= 5 && "Dishonest prover (honest only when x0=0, random otherwise) should rarely pass");
    },
    
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    return 0;
}