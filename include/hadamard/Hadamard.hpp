/*
 * Author: Andy Zhu
 * @date    2025-11-12 20:11:37
 * @version 1.0.0
 */

#ifndef HADAMARD_HPP
#define HADAMARD_HPP

#include <vector>

namespace pcpp {

class Hadamard {
public:
    Hadamard();

    Hadamard(const std::vector<bool> value);

    bool query(int idx) const;
private:
    std::vector<bool> code;
};

}

#endif
