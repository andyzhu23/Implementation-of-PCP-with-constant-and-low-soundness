#include <iostream>
#include <random>

#include "constants.hpp"
#include "finite_field/constant.hpp"
#include "finite_field/FiniteFieldElement.hpp"

namespace finite_field {

FiniteFieldElement::FiniteFieldElement() : value(0) {}

FiniteFieldElement::FiniteFieldElement(int value) : value(value % FINITE_FIELD_SIZE) {}

int FiniteFieldElement::getValue() const {
    return value;
}

FiniteFieldElement::operator int() const {
    return value;
}

FiniteFieldElement FiniteFieldElement::operator+(const FiniteFieldElement &other) const {
    return FiniteFieldElement((value + other.value) % FINITE_FIELD_SIZE);
}

FiniteFieldElement FiniteFieldElement::operator-(const FiniteFieldElement &other) const {
    return FiniteFieldElement((value - other.value + FINITE_FIELD_SIZE) % FINITE_FIELD_SIZE);
}

FiniteFieldElement FiniteFieldElement::operator*(const FiniteFieldElement &other) const {
    return FiniteFieldElement(((long long)value * other.value) % FINITE_FIELD_SIZE);
}

FiniteFieldElement FiniteFieldElement::operator+=(const FiniteFieldElement &other) {
    value = (value + other.value) % FINITE_FIELD_SIZE;
    return *this;
}

FiniteFieldElement FiniteFieldElement::operator-=(const FiniteFieldElement &other) {
    value = (value - other.value + FINITE_FIELD_SIZE) % FINITE_FIELD_SIZE;
    return *this;
}

FiniteFieldElement FiniteFieldElement::operator*=(const FiniteFieldElement &other) {
    value = ((long long)value * other.value) % FINITE_FIELD_SIZE;
    return *this;
}

FiniteFieldElement FiniteFieldElement::operator/(const FiniteFieldElement &other) const {
    return *this * other.inv();
}

FiniteFieldElement FiniteFieldElement::operator/=(const FiniteFieldElement &other) {
    *this = *this / other;
    return *this;
}

FiniteFieldElement FiniteFieldElement::exp(size_t exponent) const {
    FiniteFieldElement result(1);
    FiniteFieldElement base(*this);
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

FiniteFieldElement FiniteFieldElement::inv() const {
    // Using Fermat's little theorem: a^(p-2) mod p is the multiplicative inverse of a mod p
    return exp(FINITE_FIELD_SIZE - 2);
}

FiniteFieldElement get_random_element() {
    return FiniteFieldElement(constants::RANDOM_SEED() % FINITE_FIELD_SIZE);
}

FiniteFieldElement get_primitive_root_of_unity(size_t n) {
    if ((FINITE_FIELD_SIZE - 1) % n != 0) {
        throw std::invalid_argument("n must divide FINITE_FIELD_SIZE - 1 to have a primitive root of unity");
    }
    return FINITE_FIELD_GENERATOR.exp((FINITE_FIELD_SIZE - 1) / n);
}

FiniteFieldElement get_inv_primitive_root_of_unity(size_t n) {
    if ((FINITE_FIELD_SIZE - 1) % n != 0) {
        throw std::invalid_argument("n must divide FINITE_FIELD_SIZE - 1 to have a primitive root of unity");
    }
    return FINITE_FIELD_GENERATOR.exp((FINITE_FIELD_SIZE - 1) - (FINITE_FIELD_SIZE - 1) / n);
}

}