#include <cmath>

#include "core/core.hpp"
#include "constants.hpp"
#include "three_color/three_color.hpp"

namespace core {

pcp::BitPCP three_coloring_to_BitPCP(const three_color::ThreeColor &tc) {
    pcp::BitPCP pcp = tc.to_BitPCP();
    int iterations = std::log2(tc.get_colors().size()) / constants::SCALING_FACTOR;
    for (int i = 0; i < iterations; ++i) {
        pcp = gap_amplification(pcp);
    }
    return pcp;
}

}