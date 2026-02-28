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

FiniteFieldElement FiniteFieldElement::operator*(const FiniteFieldElement &other) const {
    return FiniteFieldElement((value * other.value) % FINITE_FIELD_SIZE);
}

FiniteFieldElement FiniteFieldElement::operator+=(const FiniteFieldElement &other) {
    value = (value + other.value) % FINITE_FIELD_SIZE;
    return *this;
}

FiniteFieldElement FiniteFieldElement::operator*=(const FiniteFieldElement &other) {
    value = (value * other.value) % FINITE_FIELD_SIZE;
    return *this;
}

FiniteFieldElement get_random_element() {
    return FiniteFieldElement(constants::RANDOM_SEED() % FINITE_FIELD_SIZE);
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

}