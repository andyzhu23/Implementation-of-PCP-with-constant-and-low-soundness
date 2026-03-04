#include <stdexcept>

#include "finite_field/constant.hpp"
#include "finite_field/FiniteFieldElement.hpp"
#include "finite_field/NumberTheoreticTransform.hpp"

namespace finite_field {

void padding(std::vector<FiniteFieldElement> &input) {
    size_t size = 1;
    while (size < input.size()) {
        size <<= 1;
    }
    input.resize(size, FiniteFieldElement(0));
}

std::vector<FiniteFieldElement> ntt(std::vector<FiniteFieldElement> input) {
    padding(input);

    if ((FINITE_FIELD_SIZE - 1) % input.size() != 0) {
        throw std::invalid_argument("Input size must divide FINITE_FIELD_SIZE - 1 for NTT");
    }

    FiniteFieldElement omega = finite_field::get_primitive_root_of_unity(input.size());

    return ntt_helper(input, omega);
}

std::vector<FiniteFieldElement> intt(std::vector<FiniteFieldElement> input) {
    padding(input);

    if ((FINITE_FIELD_SIZE - 1) % input.size() != 0) {
        throw std::invalid_argument("Input size must divide FINITE_FIELD_SIZE - 1 for inverse NTT");
    }

    FiniteFieldElement omega_inv = finite_field::get_inv_primitive_root_of_unity(input.size());

    auto transformed = ntt_helper(input, omega_inv);

    // Normalize by multiplying with n^-1 mod FINITE_FIELD_SIZE
    FiniteFieldElement n_inv = FiniteFieldElement(input.size()).inv();

    for (auto &x : transformed) {
        x *= n_inv;
    }

    return transformed;
}

std::vector<FiniteFieldElement> ntt_helper(std::vector<FiniteFieldElement> input, FiniteFieldElement omega) {
    if (input.size() == 1) {
        return input;
    }

    size_t mid = input.size() / 2;

    std::vector<FiniteFieldElement> even(mid), odd(mid);
    for (size_t i = 0; i < mid; ++i) {
        even[i] = input[2 * i];
        odd[i] = input[2 * i + 1];
    }

    FiniteFieldElement omega_squared = omega * omega; // Square of the primitive root for the recursive calls

    auto even_transformed = ntt_helper(even, omega_squared);
    auto odd_transformed = ntt_helper(odd, omega_squared);

    for (size_t i = 0; i < mid; ++i) {
        FiniteFieldElement omega_i = omega.exp(i); // omega^i
        input[i] = even_transformed[i] + omega_i * odd_transformed[i];
        input[i + mid] = even_transformed[i] - omega_i * odd_transformed[i];
    }

    return input;
}

std::vector<FiniteFieldElement> convolution(std::vector<FiniteFieldElement> a, std::vector<FiniteFieldElement> b) {

    a.resize(a.size() + b.size() - 1); // Resize to hold the full convolution result
    b.resize(a.size()); // Resize b to match the new size of a

    // Pad both vectors to the next power of 2 for NTT
    padding(a);
    padding(b);

    std::vector<FiniteFieldElement> a_ntt = ntt(a);
    std::vector<FiniteFieldElement> b_ntt = ntt(b);

    for (size_t i = 0; i < a_ntt.size(); ++i) {
        a_ntt[i] *= b_ntt[i];
    }

    return intt(a_ntt);
}

}