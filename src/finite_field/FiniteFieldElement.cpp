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

}