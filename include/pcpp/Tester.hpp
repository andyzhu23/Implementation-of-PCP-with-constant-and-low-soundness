#ifndef TESTER_HPP
#define TESTER_HPP

#include "pcp/BinaryCSP.hpp"
#include "three_color/ThreeColor.hpp" 

namespace pcpp {

class Tester {
public:
    virtual pcp::BinaryCSP three_color_to_BinaryCSP(const three_color::ThreeColor &tc) = 0;

    virtual void create_tester(const pcp::BinaryCSP &powering_pcp) = 0;

    virtual pcp::BinaryCSP buildBinaryCSP() = 0;

    virtual ~Tester() = default;

};

}

#endif