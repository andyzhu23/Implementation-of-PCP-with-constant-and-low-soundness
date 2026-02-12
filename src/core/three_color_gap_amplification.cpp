#include <cmath>
#include <functional>

#include "core/core.hpp"
#include "constants.hpp"
#include "three_color/ThreeColor.hpp"


namespace core {

pcp::BitPCP three_color_gap_amplification(const three_color::ThreeColor &tc, pcpp::TesterType tester_type, const std::function<int(size_t)>& iterations_func) {
    pcp::BitPCP pcp = tc.to_BitPCP(tester_type);
    int iterations = iterations_func(tc.get_edge_size());
    iterations = std::max(iterations, 0);
    for (int i = 0; i < iterations; ++i) {
        std::cout << "Gap amplification iteration " << (i + 1) << " / " << iterations << ' ' << pcp.get_size() << ' ' << pcp.get_constraints_list().size() << std::endl;
        pcp = gap_amplification(pcp, tester_type);
    }
    return pcp;
}

}