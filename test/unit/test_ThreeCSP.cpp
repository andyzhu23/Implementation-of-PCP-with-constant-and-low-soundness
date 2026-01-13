#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

#include "three_csp/ThreeCSP.hpp"

std::vector<std::function<void()>> test_cases = {
    
};

int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}