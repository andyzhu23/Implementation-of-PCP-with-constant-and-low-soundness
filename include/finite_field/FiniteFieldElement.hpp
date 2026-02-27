#ifndef FINITE_FIELD_ELEMENT_HPP
#define FINITE_FIELD_ELEMENT_HPP

namespace finite_field {

class FiniteFieldElement {
public:

    FiniteFieldElement();

    FiniteFieldElement(int value);

    int getValue() const;

    explicit operator int() const;

    FiniteFieldElement operator+(const FiniteFieldElement &other) const;

    FiniteFieldElement operator*(const FiniteFieldElement &other) const;

    FiniteFieldElement operator+=(const FiniteFieldElement &other);

    FiniteFieldElement operator*=(const FiniteFieldElement &other);

private:
    int value;
};

}

#endif