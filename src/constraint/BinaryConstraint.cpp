/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#include <functional>

#include <constraint/BinaryConstraint.hpp>

namespace constraint {

BinaryConstraint::BinaryConstraint(std::function<bool(int, int)> *f) : f(f) {}

bool BinaryConstraint::operator()(int x, int y) const { return (*f)(x, y); }

bool BinaryConstraint::operator==(const BinaryConstraint &other) const { return f == other.f; };

bool BinaryConstraint::operator!=(const BinaryConstraint &other) const { return f != other.f; };

std::function<bool(int, int)> _BinaryANY = [](int x, int y) -> bool { return true; };
std::function<bool(int, int)> _BinaryEQUAL = [](int x, int y) -> bool { return x == y; };
std::function<bool(int, int)> _BinaryNOTEQUAL = [](int x, int y) -> bool { return x != y; };

const BinaryConstraint BinaryANY(&_BinaryANY);
const BinaryConstraint BinaryEQUAL(&_BinaryEQUAL);
const BinaryConstraint BinaryNOTEQUAL(&_BinaryNOTEQUAL);

}
