#ifndef PSEUDOTESTER_HPP
#define PSEUDOTESTER_HPP 

#include "pcpp/Tester.hpp"

namespace pcpp {

class PseudoTester : public Tester {
public:

    PseudoTester();

    PseudoTester(const pcp::BitPCP &powering_pcp);

    PseudoTester(const three_color::ThreeColor &tc);

    void create_tester(const three_color::ThreeColor &tc) override;

    void create_tester(const pcp::BitPCP &powering_pcp) override;

    pcp::BitPCP buildBitPCP() override;

private:
    pcp::BitPCP pcp;
    bool satisfiable;
};

}

#endif