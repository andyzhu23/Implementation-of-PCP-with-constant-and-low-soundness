#ifndef PCPP_UTIL_HPP
#define PCPP_UTIL_HPP

#include <memory>

#include "pcpp/Tester.hpp"
#include "pcpp/HadamardPCPP/HadamardTester.hpp"

namespace pcpp {

enum class TesterType {
    PSEUDO,
    HADAMARD,
};

std::unique_ptr<Tester> get_tester(TesterType tester_type);

}

#endif