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

    FiniteFieldElement operator-(const FiniteFieldElement &other) const;

    FiniteFieldElement operator*(const FiniteFieldElement &other) const;

    FiniteFieldElement operator/(const FiniteFieldElement &other) const;

    FiniteFieldElement operator+=(const FiniteFieldElement &other);

    FiniteFieldElement operator-=(const FiniteFieldElement &other);

    FiniteFieldElement operator*=(const FiniteFieldElement &other);

    FiniteFieldElement operator/=(const FiniteFieldElement &other);

    template <typename T>
    bool operator==(const T &other) const {
        return value == static_cast<int>(other);
    }

    FiniteFieldElement exp(size_t exponent) const;

    FiniteFieldElement inv() const; // multiplicative inverse

private:
    int value;
};

// Utility function to get a random finite field element
FiniteFieldElement get_random_element();

// Utility function to get a primitive root of unity for a given size n (where n divides FINITE_FIELD_SIZE - 1)
FiniteFieldElement get_primitive_root_of_unity(size_t n);

// Utility function to get the inverse of a primitive root of unity for a given size n
FiniteFieldElement get_inv_primitive_root_of_unity(size_t n);

}

#endif