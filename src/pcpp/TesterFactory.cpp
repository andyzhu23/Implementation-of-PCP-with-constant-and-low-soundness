#include "pcpp/TesterFactory.hpp"

namespace pcpp {

std::unique_ptr<Tester> get_tester(TesterType tester_type) {
    switch (tester_type) {
        case TesterType::PSEUDO: 
            return std::make_unique<PseudoTester>();
        case TesterType::HADAMARD:
            return std::make_unique<HadamardTester>();
        case TesterType::REEDMULLER:
            return std::make_unique<ReedMullerTester>();
    }
    return nullptr;
}

}
