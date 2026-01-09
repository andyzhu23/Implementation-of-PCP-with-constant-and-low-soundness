
#ifndef HADAMARD_HPP
#define HADAMARD_HPP

#include <vector>

#include "pcp/Aliases.hpp"

namespace pcpp {

class Hadamard {
public:
    Hadamard();

    Hadamard(const std::vector<bool> value);

    bool query(pcp::Variable idx) const;

    std::vector<bool> getCode() const;

private:
    std::vector<bool> value;
};

}

#endif
