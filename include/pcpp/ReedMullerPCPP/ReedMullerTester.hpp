#ifndef REED_MULLER_TESTER_HPP
#define REED_MULLER_TESTER_HPP

#include "pcpp/Tester.hpp"

namespace pcpp {

class ReedMullerTester : public Tester {
public:
    ReedMullerTester();

    ReedMullerTester(const pcp::BinaryCSP &powering_pcp);

    pcp::BinaryCSP three_color_to_BinaryCSP(const three_color::ThreeColor &tc) override;

    void create_tester(const pcp::BinaryCSP &powering_pcp) override;

    pcp::BinaryCSP buildBinaryCSP() override;
private:

};

}

#endif