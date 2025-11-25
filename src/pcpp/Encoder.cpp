#include "hadamard/Encoder.hpp"

namespace pcpp {

std::vector<bool> encode_constraint(const constraint::BinaryConstraint& constraint) {
    std::vector<bool> encoding(1 << (pcp::SimpleDomainSize * 2), false);
    for (size_t i = 0; i < (1 << pcp::SimpleDomainSize); ++i) {
        for (size_t j = 0; j < (1 << pcp::SimpleDomainSize); ++j) {
            pcp::SimpleDomain a(i);
            pcp::SimpleDomain b(j);
            if (constraint(a, b)) {
                encoding[(i << pcp::SimpleDomainSize) | j] = true;
            }
        }
    }
    return encoding;
}

}