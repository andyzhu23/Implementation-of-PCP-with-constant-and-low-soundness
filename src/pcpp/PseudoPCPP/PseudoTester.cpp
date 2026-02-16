#include "pcpp/PseudoPCPP/PseudoTester.hpp"
#include "pcpp/PseudoPCPP/CSPSolver.hpp"
#include "util/disjoint_set_union.hpp"

namespace pcpp {

PseudoTester::PseudoTester() {}

PseudoTester::PseudoTester(const pcp::BitPCP &powering_pcp) : pcp(powering_pcp) {}

PseudoTester::PseudoTester(const three_color::ThreeColor &tc) : pcp(tc.get_colors().size()) {
    for (three_color::Node u = 0; u < tc.get_colors().size(); ++u) {
        for (three_color::Node v : tc.get_adj_list()[u]) if (v > u) {
            pcp.add_constraint(u, v, constraint::BitConstraint::NOTEQUAL);
        }
        switch (tc.get_colors()[u]) {
            case three_color::Color::RED:
                pcp.set_variable(u, pcp::BitDomain(0, 0, 1, three_csp::Constraint::ONE_HOT_COLOR));
                break;
            case three_color::Color::GREEN:
                pcp.set_variable(u, pcp::BitDomain(0, 1, 0, three_csp::Constraint::ONE_HOT_COLOR));
                break;
            case three_color::Color::BLUE:
                pcp.set_variable(u, pcp::BitDomain(1, 0, 0, three_csp::Constraint::ONE_HOT_COLOR));
                break;
        }
    }
}

pcp::BitPCP PseudoTester::three_color_to_bitpcp(const three_color::ThreeColor &tc) {
    PseudoTester ptester(tc);
    return ptester.pcp;
}

void PseudoTester::create_tester(const pcp::BitPCP &powering_pcp) {
    PseudoTester ptester(powering_pcp);
    ptester.satisfiable = check_bitPCP_satisfiability(ptester.pcp);
    *this = std::move(ptester);
}

pcp::BitPCP PseudoTester::buildBitPCP() {
    size_t size = 32;

    pcp::BitPCP hardcoded_pcpp(size);

    for (size_t i = 0; i < size; ++i) {
        hardcoded_pcpp.set_variable(i, pcp::BitDomain(0, 0, 0, three_csp::Constraint::ENCODED_BINARY));
    }

    for (size_t i = 1; i < size; ++i) {
        hardcoded_pcpp.add_constraint(i, i - 1, constraint::BitConstraint::EQUAL);
    }

    if (satisfiable) {
        hardcoded_pcpp.add_constraint(0, 1, constraint::BitConstraint::EQUAL);
    } else {
        hardcoded_pcpp.add_constraint(0, 1, constraint::BitConstraint::NOTEQUAL);
    }

    return hardcoded_pcpp;
}

}