#include "hadamard/Hadamard.hpp"

namespace pcpp {

Hadamard::Hadamard() {}

Hadamard::Hadamard(const std::vector<bool> value) : code(1 << value.size()) {
    for (size_t i = 0; i < code.size(); ++i) {
        for (size_t j = 0; j < value.size(); ++j) {
            code[i] = (((i >> j) & 1) & value[j]) ? (!code[i]) : code[i];
        }
    }
}

bool Hadamard::query(int idx) const {
    return code[idx];
}

const std::vector<bool>& Hadamard::getCode() const {
    return code;
}

}