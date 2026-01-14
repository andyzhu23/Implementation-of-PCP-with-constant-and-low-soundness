#include <cmath>
#include <future>
#include <vector>
#include <mutex>

#include "core/core.hpp"
#include "constants.hpp"
#include "pcpp/Tester.hpp"

namespace core {

pcp::BitPCP gap_amplification(pcp::BitPCP pcp) {
    to_expander(pcp, constants::EXPANDING_COEFFICIENT);
    pcp = reduce_degree(pcp, constants::DEGREE);

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = constants::SAFE_THREAD_NUMBER;


    size_t total_size = pcp.get_size();

    std::vector<std::vector<std::pair<size_t, size_t>>> occuring_locations(total_size);

    std::vector<pcp::BitPCP> reduced_pcps(total_size);

    std::vector<std::future<void>> futures;
    size_t chunk_size = (total_size + num_threads - 1) / num_threads;

    // for updating occuring locations of variables
    std::mutex mtx;

    for (unsigned int t = 0; t < num_threads; ++t) {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, total_size);

        if (start >= total_size) break;

        futures.push_back(std::async(std::launch::async,
            [&reduced_pcps, start, end, &pcp, &mtx, &occuring_locations]() {
                for (size_t u = start; u < end; ++u) {
                    std::vector<pcp::Variable> neighbors = pcp.get_neighbors(u, constants::POWERING_RADIUS);
                    for (size_t i = 0; i < neighbors.size(); ++i) if (neighbors[i] < u) {
                        std::lock_guard<std::mutex> lock(mtx);
                        occuring_locations[neighbors[i]].emplace_back(u, i);
                    }
                    pcp::BitPCP powering_u = pcp.build_sub_pcp(neighbors);
                    pcp::BitPCP reduced_pcp = pcpp::Tester(powering_u).buildBitPCP();
                    // reduced_pcp.clean();
                    reduced_pcps[u] = std::move(reduced_pcp);
                }
            }
        ));
    }
    for (auto &f : futures) {
        f.get();
    }

    pcp = pcp::merge_BitPCPs(reduced_pcps);

    std::vector<pcp::Variable> offset(total_size, 0);
    
    for (pcp::Variable u = 0; u < static_cast<pcp::Variable>(total_size); ++u) {
        if (u < total_size) {
            offset[u + 1] = offset[u] + reduced_pcps[u].get_size();
        }
    }

    std::uniform_int_distribution<size_t> dist(0, total_size - 1);
    // add consistency enforcement constraints
    for (size_t _ = 0; _ < constants::CONSISTENCY_ENFORCEMENT_REPETITION; ++_) {
        size_t u = dist(constants::RANDOM_SEED);
        const auto &locations = occuring_locations[u];
        if (locations.size() < 2) continue;
        std::uniform_int_distribution<size_t> loc_dist(0, locations.size() - 1);
        size_t loc1 = loc_dist(constants::RANDOM_SEED);
        size_t loc2 = loc_dist(constants::RANDOM_SEED);
        while (loc2 == loc1) {
            loc2 = loc_dist(constants::RANDOM_SEED);
        }
        pcp.add_constraint(
            offset[locations[loc1].first] + locations[loc1].second,
            offset[locations[loc2].first] + locations[loc2].second,
            constraint::BitConstraint::EQUAL
        );
    }

    pcp.clean();
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