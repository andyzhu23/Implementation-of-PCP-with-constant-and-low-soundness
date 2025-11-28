#include <functional>
#include <iostream>

std::vector<std::function<void()>> test_cases = {
    []() -> void {

    },
};


int main() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        test_cases[i]();
        std::cout << "Passed test case " << (i + 1) << std::endl;
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}