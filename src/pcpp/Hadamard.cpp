#include <stdexcept>

#include "constants.hpp"
#include "pcpp/Hadamard.hpp"

namespace pcpp {

Hadamard::Hadamard() {}

Hadamard::Hadamard(const std::vector<bool> value) : value(value) {}

bool Hadamard::query(size_t idx) const {
    bool code = false;
    for (size_t i = 0; i < value.size(); ++i) {
        code = (((idx >> i) & 1) & value[i]) ? (!code) : code;
    }   
    return code;
}

std::vector<bool> Hadamard::getCode() const {
    std::vector<bool> code(constants::PCPVARIABLE_ONE << value.size());
    for (size_t i = 0; i < code.size(); ++i) {
        code[i] = query(i);
    }
    return code;
}

}