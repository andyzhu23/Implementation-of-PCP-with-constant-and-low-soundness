#include "pcpp/TesterFactory.hpp"

namespace pcpp {

std::unique_ptr<Tester> get_tester(TesterType tester_type) {
    switch (tester_type) {
        case TesterType::PSEUDO: 
            return std::make_unique<PseudoTester>();
        case TesterType::HADAMARD:
            return std::make_unique<HadamardTester>();
    }
    return nullptr;
}

}
