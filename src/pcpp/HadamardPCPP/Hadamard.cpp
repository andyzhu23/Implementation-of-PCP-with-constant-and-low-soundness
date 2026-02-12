#include <stdexcept>

#include "Aliases.hpp"
#include "constants.hpp"
#include "pcpp/HadamardPCPP/Hadamard.hpp"

namespace pcpp {

Hadamard::Hadamard() {}

Hadamard::Hadamard(const std::vector<bool> value) : value(value) {}

bool Hadamard::query(const std::vector<bool> &idx) const {
    bool code = false;
    for (size_t i = 0; i < value.size(); ++i) {
        code = (idx[i] & value[i]) ? (!code) : code;
    }
    return code;
}

bool Hadamard::query(std::vector<bool> &&idx) const {
    bool code = false;
    for (size_t i = 0; i < value.size(); ++i) {
        code = (idx[i] & value[i]) ? (!code) : code;
    }
    return code;
}

std::vector<bool> Hadamard::getCode() const {
    std::vector<bool> code(constants::PCPVARIABLE_ONE << value.size());
    for (size_t i = 0; i < code.size(); ++i) {
        std::vector<bool> idx(value.size());
        for (size_t j = 0; j < value.size(); ++j) {
            idx[j] = (i >> j) & 1;
        }
        code[i] = query(idx);
    }
    return code;
}

}