#include "pcp/BinaryDomain.hpp"

namespace pcp {

BinaryDomain::BinaryDomain() : bits(0), domain_type(three_csp::Constraint::ANY) {}

BinaryDomain::BinaryDomain(int value) 
 : bits(value), domain_type(three_csp::Constraint::ANY) {}

BinaryDomain::BinaryDomain(int value, three_csp::Constraint domain_type)
 : bits(value), domain_type(domain_type) {}


BinaryDomain::BinaryDomain(bool bit0, bool bit1, bool bit2, three_csp::Constraint domain_type)
 : bits(0), domain_type(domain_type) {
    bits[0] = bit0;
    bits[1] = bit1;
    bits[2] = bit2;
}

bool BinaryDomain::operator==(const BinaryDomain &other) const {
    return bits == other.bits && domain_type == other.domain_type;
}

bool BinaryDomain::operator!=(const BinaryDomain &other) const {
    return !(*this == other);
}

bool BinaryDomain::operator[](size_t index) const {
    return bits[index];
}

three_csp::Constraint BinaryDomain::get_domain_type() const {
    return domain_type;
}

void BinaryDomain::set_domain_type(three_csp::Constraint new_domain_type) {
    domain_type = new_domain_type;
}

}