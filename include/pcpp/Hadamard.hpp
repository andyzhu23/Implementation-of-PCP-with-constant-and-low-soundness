
#ifndef HADAMARD_HPP
#define HADAMARD_HPP

#include <vector>

namespace pcpp {

class Hadamard {
public:
    Hadamard();

    Hadamard(const std::vector<bool> value);

    bool query(int idx) const;

    const std::vector<bool>& getCode() const;

private:
    std::vector<bool> code;
};

}

#endif
