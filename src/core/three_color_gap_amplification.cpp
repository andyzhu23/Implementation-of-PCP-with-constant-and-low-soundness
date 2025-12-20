#include <cmath>

#include "core/core.hpp"
#include "constants.hpp"
#include "three_color/three_color.hpp"

#include <iostream>

namespace core {

pcp::BitPCP three_color_gap_amplification(const three_color::ThreeColor &tc) {
    pcp::BitPCP pcp = tc.to_BitPCP();
    int iterations = std::ceil(std::log10(tc.get_colors().size()));
    for (int i = 0; i < iterations; ++i) {
        std::cout << "Gap amplification iteration " << (i + 1) << " / " << iterations << ' ' << pcp.get_size() << std::endl;
        pcp = gap_amplification(pcp);
    }
    return pcp;
}

}