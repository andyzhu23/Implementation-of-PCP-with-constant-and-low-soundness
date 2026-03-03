#ifndef BinaryDomain_HPP
#define BinaryDomain_HPP

#include <bitset>

#include "Aliases.hpp"

namespace pcp {

class BinaryDomain {
public:
    BinaryDomain();

    BinaryDomain(int value);

    BinaryDomain(int value, three_csp::Constraint domain_type);

    BinaryDomain(bool bit0, bool bit1, bool bit2, three_csp::Constraint domain_type);

    bool operator==(const BinaryDomain &other) const;

    bool operator!=(const BinaryDomain &other) const;

    bool operator[](size_t index) const;

    three_csp::Constraint get_domain_type() const;

    void set_domain_type(three_csp::Constraint new_domain_type);

private:
    std::bitset<3> bits;
    three_csp::Constraint domain_type;
};

}

#endif