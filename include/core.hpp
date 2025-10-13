#ifndef CORE_HPP
#define CORE_HPP

#include "csp.hpp"

namespace core {
    template <typename D>
    csp::CSP<std::vector<D>> power_operation(csp::CSP<D> p, int r);
}

#endif
