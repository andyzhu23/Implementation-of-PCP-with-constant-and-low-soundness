#ifndef TESTER_HPP
#define TESTER_HPP

#include "pcp/BitPCP.hpp"
#include "three_color/ThreeColor.hpp" 

namespace pcpp {

class Tester {
public:
    virtual void create_tester(const three_color::ThreeColor &tc) = 0;

    virtual void create_tester(const pcp::BitPCP &powering_pcp) = 0;

    virtual pcp::BitPCP buildBitPCP() = 0;

    virtual ~Tester() = default;

};

}

#endif