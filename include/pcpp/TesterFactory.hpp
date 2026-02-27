#ifndef PCPP_UTIL_HPP
#define PCPP_UTIL_HPP

#include <memory>

#include "pcpp/Tester.hpp"
#include "pcpp/HadamardPCPP/HadamardTester.hpp"
#include "pcpp/PseudoPCPP/PseudoTester.hpp"
#include "pcpp/ReedMullerPCPP/ReedMullerTester.hpp"

namespace pcpp {

enum class TesterType {
    PSEUDO,
    HADAMARD,
    REEDMULLER,
};

std::unique_ptr<Tester> get_tester(TesterType tester_type);

}

#endif