#ifndef REED_MULLER_TESTER_HPP
#define REED_MULLER_TESTER_HPP

#include "pcpp/Tester.hpp"

namespace pcpp {

class ReedMullerTester : public Tester {
public:
    ReedMullerTester();

    ReedMullerTester(const pcp::BitPCP &powering_pcp);

    pcp::BitPCP three_color_to_bitpcp(const three_color::ThreeColor &tc) override;

    void create_tester(const pcp::BitPCP &powering_pcp) override;

    pcp::BitPCP buildBitPCP() override;
private:

};

}

#endif