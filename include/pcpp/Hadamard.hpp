
#ifndef HADAMARD_HPP
#define HADAMARD_HPP

#include <vector>

#include "pcp/Aliases.hpp"

namespace pcpp {

class Hadamard {
public:
    Hadamard();

    Hadamard(const std::vector<bool> value);

    bool query(const std::vector<bool>& idx) const;

    bool query(std::vector<bool>&& idx) const;

    std::vector<bool> getCode() const;

private:
    std::vector<bool> value;
};

}

#endif
