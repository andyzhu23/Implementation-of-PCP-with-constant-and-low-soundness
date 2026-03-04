#ifndef NUMBERTHEORETICTRANSFORM_HPP
#define NUMBERTHEORETICTRANSFORM_HPP

#include <vector>

#include "FiniteFieldElement.hpp"

namespace finite_field {

void padding(std::vector<FiniteFieldElement> &input);

std::vector<FiniteFieldElement> ntt(std::vector<FiniteFieldElement> input);

std::vector<FiniteFieldElement> intt(std::vector<FiniteFieldElement> input);

std::vector<FiniteFieldElement> ntt_helper(std::vector<FiniteFieldElement> input, FiniteFieldElement omega);

std::vector<FiniteFieldElement> convolution(std::vector<FiniteFieldElement> a, std::vector<FiniteFieldElement> b);

}

#endif