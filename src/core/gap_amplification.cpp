#include <cmath>
#include <future>
#include <vector>

#include "core/core.hpp"
#include "constants.hpp"
#include "pcpp/Tester.hpp"

#include <iostream>

namespace core {

pcp::BitPCP gap_amplification(pcp::BitPCP pcp) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = constants::SAFE_THREAD_NUMBER;

    size_t total_size = pcp.get_size();

    std::vector<pcp::BitPCP> reduced_pcps(total_size);

    std::vector<std::future<void>> futures;
    size_t chunk_size = (total_size + num_threads - 1) / num_threads;

    for (unsigned int t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, total_size);

        if (start >= total_size) break;

        futures.push_back(std::async(std::launch::async, 
            [&pcp, &reduced_pcps, start, end]() {
                for (size_t u = start; u < end; ++u) {
                    pcp::BitPCP powering_u = pcp.get_neighboring_pcp(u, constants::POWERING_RADIUS);
                    pcp::BitPCP reduced_pcp = pcpp::Tester(powering_u).buildBitPCP();
                    reduced_pcp.clean();
                    reduced_pcps[u] = std::move(reduced_pcp);
                }
            }
        ));
    }

    for (auto &f : futures) {
        f.get();
    }
    
    pcp = pcp::merge_BitPCPs(reduced_pcps);
    return pcp;
}

}

// namespace core {

// pcp::BitPCP gap_amplification(pcp::BitPCP pcp) {
//     to_expander(pcp, constants::EXPANDING_COEFFICIENT);
//     pcp = reduce_degree(pcp, constants::DEGREE);

//     std::vector<pcp::BitPCP> reduced_pcps;

//     for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(pcp.get_size()); ++u) {
//         pcp::BitPCP powering_u = pcp.get_neighboring_pcp(u, constants::POWERING_RADIUS);
//         pcpp::Tester tester(powering_u);
//         pcp::BitPCP reduced_pcp = tester.buildBitPCP();
//         reduced_pcp.clean();
//         reduced_pcps.push_back(reduced_pcp);
//     }
//     pcp = pcp::merge_BitPCPs(reduced_pcps);

//     return pcp;
// }

// }