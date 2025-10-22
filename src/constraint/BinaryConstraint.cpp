/*
 * Author: Andy Zhu
 * @date    2025-10-22 14:19:07
 * @version 1.0.0
 */

#include <functional>

#include "constraint/BinaryConstraint.hpp"
#include "pcp/SimplePCP.hpp"

namespace constraint {

BinaryConstraint::BinaryConstraint(const std::function<bool(pcp::SimpleDomain, pcp::SimpleDomain)> *f)
     : f(f) {}

bool BinaryConstraint::operator()(pcp::SimpleDomain x, pcp::SimpleDomain y) const { return (*f)(x, y); }

bool BinaryConstraint::operator==(const BinaryConstraint &other) const { return f == other.f; };

bool BinaryConstraint::operator!=(const BinaryConstraint &other) const { return f != other.f; };

const std::function<bool(pcp::SimpleDomain, pcp::SimpleDomain)> _BinaryANY = 
    [](pcp::SimpleDomain x, pcp::SimpleDomain y) -> bool { return true; };
const std::function<bool(pcp::SimpleDomain, pcp::SimpleDomain)> _BinaryEQUAL = 
    [](pcp::SimpleDomain x, pcp::SimpleDomain y) -> bool { return x == y; };
const std::function<bool(pcp::SimpleDomain, pcp::SimpleDomain)> _BinaryNOTEQUAL = 
    [](pcp::SimpleDomain x, pcp::SimpleDomain y) -> bool { return x != y; };

const BinaryConstraint BinaryANY(&_BinaryANY);
const BinaryConstraint BinaryEQUAL(&_BinaryEQUAL);
const BinaryConstraint BinaryNOTEQUAL(&_BinaryNOTEQUAL);

}
