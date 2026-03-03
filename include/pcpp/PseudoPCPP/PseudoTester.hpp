#ifndef PSEUDOTESTER_HPP
#define PSEUDOTESTER_HPP 

#include "pcpp/Tester.hpp"

namespace pcpp {

class PseudoTester : public Tester {
public:

    PseudoTester();

    PseudoTester(const pcp::BinaryCSP &powering_pcp);

    PseudoTester(const three_color::ThreeColor &tc);

    pcp::BinaryCSP three_color_to_BinaryCSP(const three_color::ThreeColor &tc) override;

    void create_tester(const pcp::BinaryCSP &powering_pcp) override;

    pcp::BinaryCSP buildBinaryCSP() override;

private:
    pcp::BinaryCSP pcp;
    bool satisfiable;
};

}

#endif