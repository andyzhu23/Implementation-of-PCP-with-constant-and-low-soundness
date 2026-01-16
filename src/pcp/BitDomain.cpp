#include "pcp/BitDomain.hpp"

namespace pcp {

BitDomain::BitDomain() : bits(0), domain_type(three_csp::Constraint::ANY) {}

BitDomain::BitDomain(int value) 
 : bits(value), domain_type(three_csp::Constraint::ANY) {}

BitDomain::BitDomain(int value, three_csp::Constraint domain_type)
 : bits(value), domain_type(domain_type) {}


BitDomain::BitDomain(bool bit0, bool bit1, bool bit2, three_csp::Constraint domain_type)
 : bits(0), domain_type(domain_type) {
    bits[0] = bit0;
    bits[1] = bit1;
    bits[2] = bit2;
}

bool BitDomain::operator==(const BitDomain &other) const {
    return bits == other.bits && domain_type == other.domain_type;
}

bool BitDomain::operator!=(const BitDomain &other) const {
    return !(*this == other);
}

bool BitDomain::operator[](size_t index) const {
    return bits[index];
}

three_csp::Constraint BitDomain::get_domain_type() const {
    return domain_type;
}

void BitDomain::set_domain_type(three_csp::Constraint new_domain_type) {
    domain_type = new_domain_type;
}

}