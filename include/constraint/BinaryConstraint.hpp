
/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#ifndef BINARYCONSTRAINT_HPP
#define BINARYCONSTRAINT_HPP

#include <functional>

namespace constraint {

class BinaryConstraint {
public:
    BinaryConstraint(const std::function<bool(int, int)> *f);

    bool operator()(int x, int y) const;

    bool operator==(const BinaryConstraint &other) const;

    bool operator!=(const BinaryConstraint &other) const;

private:
    const std::function<bool(int, int)> *f;
};

extern const BinaryConstraint BinaryANY;
extern const BinaryConstraint BinaryEQUAL;
extern const BinaryConstraint BinaryNOTEQUAL;

}


#endif

