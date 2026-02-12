#ifndef TESTER_HPP
#define TESTER_HPP

#include "pcp/BitPCP.hpp"
#include "three_color/ThreeColor.hpp" 

namespace pcpp {

class Tester {
public:
    virtual void create_tester(three_color::Color u, three_color::Color v) = 0;

    virtual void create_tester(const pcp::BitPCP &powering_pcp) = 0;

    virtual pcp::BitPCP buildBitPCP() = 0;

    virtual ~Tester() = default;

};

}

#endif